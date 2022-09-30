#ifndef STONKS_NETWORK_NETWORK_I_WEB_SOCKET_HANDLER_H_
#define STONKS_NETWORK_NETWORK_I_WEB_SOCKET_HANDLER_H_

#include "network_types.h"

namespace stonks::network {
/**
 * @brief Handles web socket messages.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IWebSocketHandler {
 public:
  virtual ~IWebSocketHandler() = default;

  /**
   * @param message Message received from the web socket.
   */
  virtual void HandleMessage(WsMessage message) const = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_WEB_SOCKET_HANDLER_H_
