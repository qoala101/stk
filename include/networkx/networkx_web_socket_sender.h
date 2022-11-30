#ifndef STONKS_NETWORKX_NETWORKX_WEB_SOCKET_SENDER_H_
#define STONKS_NETWORKX_NETWORKX_WEB_SOCKET_SENDER_H_

#include "network_concepts.h"  // IWYU pragma: keep
#include "network_ws_connection.h"

namespace stonks::networkx {
/**
 * @brief Handle to web socket connection which allows to send objects
 * of specified type.
 * Would be passed tp SetSender of web socket type right after connecting.
 */
template <network::Convertible SendsTypeT>
class WebSocketSender {
 public:
  using SendsType = SendsTypeT;

  /**
   * @param ws_connection Connection through which to send objects.
   */
  explicit WebSocketSender(cpp::NnSp<network::WsConnection> ws_connection)
      : ws_connection_{std::move(ws_connection)} {}

  /**
   * @brief Sends object to the connected socket.
   */
  template <std::convertible_to<SendsType> TypeT>
  auto Send [[nodiscard]] (TypeT &&object) const -> cppcoro::task<> {
    co_await ws_connection_->Send(std::forward<TypeT>(object));
  }

 private:
  cpp::NnSp<network::WsConnection> ws_connection_;
};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_WEB_SOCKET_SENDER_H_
