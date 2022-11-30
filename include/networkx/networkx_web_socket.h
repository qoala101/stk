#ifndef STONKS_NETWORKX_NETWORKX_WEB_SOCKET_H_
#define STONKS_NETWORKX_NETWORKX_WEB_SOCKET_H_

#include <callable.hpp>
#include <member_function.hpp>

#include "cpp_not_null.h"
#include "network_typed_endpoint.h"
#include "network_typed_ws_endpoint.h"
#include "network_ws_client_builder.h"
#include "network_ws_connection.h"
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_web_socket_sender.h"

namespace stonks::networkx {
namespace detail {
template <typename Target>
struct ReceiverTraits {};

template <ReceivesFromWebSocket Target>
struct ReceiverTraits<Target> {
  using ReceivesType = ArgType<&Target::Receive>;
};

template <typename Target>
struct SenderTraits {};

template <SendsToWebSocket Target>
struct SenderTraits<Target> {
  using SendsType = typename ArgType<&Target::SetSender>::SendsType;
};
}  // namespace detail

/**
 * @brief API to build strongly typed web socket client.
 * Owns an object of web socket type which is responsible for providing
 * an endpoint and optionally receiving and sending messages.
 * @tparam Target Must provide endpoint via GetEndpoint.
 * Can receive messages on Receive which accepts a single object to be parsed
 * from each incoming message.
 * Can get a message sender of specified type via SetReceiver.
 */
template <WebSocketType Target>
class WebSocket {
 private:
  using ReceiverTraits = detail::ReceiverTraits<Target>;
  using SenderTraits = detail::SenderTraits<Target>;

  static const auto kReceivesFromWebSocket = ReceivesFromWebSocket<Target>;
  static const auto kSendsToWebSocket = SendsToWebSocket<Target>;

 public:
  /**
   * @brief Connects to web socket specified by endpoint specified by target
   * and redirects incoming messages to its Receive function.
   * Calls SetSender with required sender if target provides it.
   */
  WebSocket(cpp::NnUp<Target> target, cpp::NnUp<network::IWsClient> ws_client)
      : ws_connection_{[parent = cpp::NnSp<Target>{std::move(target)},
                        &ws_client]() {
          auto ws_client_builder =
              network::WsClientBuilder{{.endpoint = parent->GetEndpoint(),
                                        .expected_types = ExpectedTypes()},
                                       std::move(ws_client)};

          if constexpr (kReceivesFromWebSocket) {
            ws_client_builder.Handling(
                [parent](auto message) -> cppcoro::task<> {
                  co_await parent->Receive(*message);
                });
          }

          auto ws_connection =
              cpp::MakeNnSp<network::WsConnection>(ws_client_builder.Connect());

          if constexpr (kSendsToWebSocket) {
            parent->SetSender(WebSocketSender<typename SenderTraits::SendsType>{
                ws_connection});
          }

          return ws_connection;
        }()} {}

 private:
  static auto ExpectedTypes() -> network::WsEndpointTypes {
    auto expected_types = network::WsEndpointTypes{};

    if constexpr (kReceivesFromWebSocket) {
      expected_types.received_message =
          network::ExpectedType<typename ReceiverTraits::ReceivesType>();
    }

    if constexpr (kSendsToWebSocket) {
      expected_types.sent_message =
          network::ExpectedType<typename SenderTraits::SendsType>();
    }

    return expected_types;
  }

  cpp::NnSp<network::WsConnection> ws_connection_;
};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_WEB_SOCKET_H_
