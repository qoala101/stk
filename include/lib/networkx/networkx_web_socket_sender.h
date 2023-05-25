/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORKX_WEB_SOCKET_SENDER_H_
#define VH_NETWORKX_WEB_SOCKET_SENDER_H_

#include <concepts>
#include <cppcoro/task.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_ws_client.h"
#include "network_typed_endpoint.h"
#include "network_ws_connection.h"
#include "network_ws_types.h"

namespace vh::networkx {
namespace detail {
class WebSocketSenderBase {
 protected:
  explicit WebSocketSenderBase(network::WsConnection connection);

  WebSocketSenderBase(network::WsEndpoint endpoint,
                      cpp::NnUp<network::IWsClient> ws_client,
                      network::ParseTypeCheck sent_message_type);

  auto GetConnection [[nodiscard]] () const -> const network::WsConnection &;

 private:
  network::WsConnection connection_;
};
}  // namespace detail

/**
 * @brief Web socket connection which can only send objects of specified type.
 */
template <network::Convertible SendsTypeT>
class WebSocketSender : public detail::WebSocketSenderBase {
 public:
  using SendsType = SendsTypeT;

  /**
   * @brief Wraps existing connection.
   */
  explicit WebSocketSender(network::WsConnection connection)
      : detail::WebSocketSenderBase{std::move(connection)} {}

  /**
   * @brief Connects to the specified endpoint.
   */
  WebSocketSender(network::WsEndpoint endpoint,
                  cpp::NnUp<network::IWsClient> ws_client)
      : detail::WebSocketSenderBase{std::move(endpoint), std::move(ws_client),
                                    network::ExpectedType<SendsType>()} {}

  /**
   * @brief Sends object to the connected socket.
   */
  template <std::convertible_to<SendsType> T>
  auto Send [[nodiscard]] (T &&message_object) const -> cppcoro::task<> {
    co_await GetConnection().Send(std::forward<T>(message_object));
  }
};
}  // namespace vh::networkx

#endif  // VH_NETWORKX_WEB_SOCKET_SENDER_H_
