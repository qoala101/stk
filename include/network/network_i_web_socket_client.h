#ifndef STONKS_NETWORK_NETWORK_I_WEB_SOCKET_CLIENT_H_
#define STONKS_NETWORK_NETWORK_I_WEB_SOCKET_CLIENT_H_

#include "cpp_not_null.h"
#include "network_i_web_socket_handler.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Allows communication with an arbitrary web socket.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IWebSocketClient {
 public:
  virtual ~IWebSocketClient() = default;

  /**
   * @brief Connects to the web socket on the specified URI
   * and redirects received messages to the handler.
   */
  virtual void Connect(Uri uri, cpp::NnSp<IWebSocketHandler> handler) = 0;

  /**
   * @brief Sends the message to the connected socket.
   */
  virtual void SendMessage(Message message) const = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_WEB_SOCKET_CLIENT_H_
