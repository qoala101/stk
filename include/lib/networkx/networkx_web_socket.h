#ifndef STONKS_NETWORKX_NETWORKX_WEB_SOCKET_H_
#define STONKS_NETWORKX_NETWORKX_WEB_SOCKET_H_

#include <concepts>
#include <gsl/assert>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "network_i_ws_client.h"
#include "network_typed_ws_endpoint.h"
#include "network_ws_client_builder.h"
#include "network_ws_connection.h"
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_web_socket_sender.h"

namespace vh::networkx {
template <cpp::MemberFunction auto F>
class WebSocket;

/**
 * @brief Convenience API which connects to specified web socket and redirects
 * received objects of specified type to receiver.
 */
template <cpp::MemberFunction auto kFunction>
  requires WebSocketReceiver<kFunction>
class WebSocket<kFunction> {
 private:
  using Parent = ParentType<kFunction>;
  using ReceivesType = ArgType<kFunction, 0>;

 public:
  /**
   * @param parent Receiver function parent.
   */
  WebSocket(network::WsEndpoint endpoint,
            cpp::NnUp<network::IWsClient> ws_client, Parent parent)
      : connection_{[&endpoint, &ws_client, &parent]() {
          return network::WsClientBuilder{{.endpoint = std::move(endpoint),
                                           .expected_types = EndpointTypes()},
                                          std::move(ws_client)}
              .Handling(ReceiverCaller(std::move(parent)))
              .Connect();
        }()} {}

 private:
  static auto EndpointTypes [[nodiscard]] () {
    return network::WsEndpointTypes{.received_message =
                                        network::ExpectedType<ReceivesType>()};
  }

  static auto ReceiverCaller [[nodiscard]] (Parent parent) {
    return
        [parent = std::move(parent)](auto message) mutable -> cppcoro::task<> {
          co_await (parent.*kFunction)(*message);
        };
  }

  network::WsConnection connection_;
};

/**
 * @brief Convenience API which connects to specified web socket and redirects
 * received objects of specified type to receiver with ability to reply.
 */
template <cpp::MemberFunction auto kFunction>
  requires WebSocketReceiverSender<kFunction>
class WebSocket<kFunction> {
 private:
  using Parent = ParentType<kFunction>;
  using ReceivesType = ArgType<kFunction, 0>;
  using SendsType =
      typename std::remove_cvref_t<ArgType<kFunction, 1>>::SendsType;
  using Sender = WebSocketSender<SendsType>;

 public:
  /**
   * @param parent Receiver function parent.
   */
  WebSocket(network::WsEndpoint endpoint,
            cpp::NnUp<network::IWsClient> ws_client, Parent parent)
      : sender_{[&endpoint, &ws_client, &parent]() {
          auto sender = cpp::MakeNnSp<cpp::Opt<Sender>>();

          *sender = Sender{
              network::WsClientBuilder{{.endpoint = std::move(endpoint),
                                        .expected_types = EndpointTypes()},
                                       std::move(ws_client)}
                  .Handling(ReceiverCaller(std::move(parent), sender))
                  .Connect()};

          return sender;
        }()} {
    Ensures(sender_->has_value());
  }

  /**
   * @brief Gives sender which allows to send objects to the connected socket.
   */
  auto GetSender [[nodiscard]] () const -> auto& {
    Expects(sender_->has_value());
    return **sender_;
  }

 private:
  static auto EndpointTypes() {
    return network::WsEndpointTypes{
        .received_message = network::ExpectedType<ReceivesType>(),
        .sent_message = network::ExpectedType<SendsType>()};
  }

  static auto ReceiverCaller
      [[nodiscard]] (Parent parent, cpp::NnSp<cpp::Opt<Sender>> sender) {
    return [parent = std::move(parent),
            sender = std::move(sender)](auto message) -> cppcoro::task<> {
      Expects(sender->has_value());
      co_await (parent.*kFunction)(*message, **sender);
    };
  }

  cpp::NnSp<cpp::Opt<Sender>> sender_;
};
}  // namespace vh::networkx

#endif  // STONKS_NETWORKX_NETWORKX_WEB_SOCKET_H_
