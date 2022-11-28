#ifndef STONKS_NETWORK_NETWORK_I_WS_CLIENT_H_
#define STONKS_NETWORK_NETWORK_I_WS_CLIENT_H_

#include <cppcoro/task.hpp>

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
   * @brief Receives message from the connected socket.
   */
  virtual auto ReceiveMessage [[nodiscard]] () -> cppcoro::task<WsMessage> = 0;

  /**
   * @brief Sends message to the connected socket.
   */
  virtual auto SendMessage [[nodiscard]] (WsMessage message) const
      -> cppcoro::task<> = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_WS_CLIENT_H_
