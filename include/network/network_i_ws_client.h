#ifndef STONKS_NETWORK_NETWORK_I_WS_CLIENT_H_
#define STONKS_NETWORK_NETWORK_I_WS_CLIENT_H_

#include "cpp_not_null.h"
#include "network_i_ws_message_handler.h"
#include "network_ws_types.h"

namespace stonks::network {
/**
 * @brief Allows communication with an arbitrary web socket.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IWsClient {
 public:
  virtual ~IWsClient() = default;

  /**
   * @brief Connects to the web socket on the specified URI.
   */
  virtual void Connect(WsEndpoint endpoint) = 0;

  /**
   * @brief Redirects received messages to the handler.
   */
  virtual void SetMessageHandler(cpp::NnUp<IWsMessageHandler> handler) = 0;

  /**
   * @brief Sends the message to the connected socket.
   */
  virtual void SendMessage(WsMessage message) const = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_WS_CLIENT_H_
