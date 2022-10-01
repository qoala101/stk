#ifndef STONKS_NETWORK_NETWORK_WEB_SOCKET_SENDER_H_
#define STONKS_NETWORK_NETWORK_WEB_SOCKET_SENDER_H_

#include <utility>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_web_socket_client.h"
#include "network_types.h"

namespace stonks::network {
class WebSocketClientBuilder;

/**
 * @brief Web socket message sender.
 */
class WebSocketSender {
 public:
  /**
   * @brief Sends the object to the previously connected web socket.
   */
  template <Convertible T>
  void Send(T &&t) const {
    return Send(ConvertToJson(std::forward<T>(t)));
  }

 private:
  friend class cpp::ExposePrivateConstructorsTo<WebSocketClientBuilder,
                                                WebSocketSender>;

  explicit WebSocketSender(cpp::NnUp<IWebSocketClient> web_socket_client);

  void Send(WsMessage message) const;

  cpp::NnUp<IWebSocketClient> web_socket_client_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_WEB_SOCKET_SENDER_H_
