#ifndef STONKS_NETWORK_NETWORK_I_WS_MESSAGE_HANDLER_H_
#define STONKS_NETWORK_NETWORK_I_WS_MESSAGE_HANDLER_H_

#include <cppcoro/task.hpp>

#include "network_ws_types.h"

namespace stonks::network {
/**
 * @brief Handles web socket messages.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IWsMessageHandler {
 public:
  virtual ~IWsMessageHandler() = default;

  /**
   * @param message Message received from the web socket.
   */
  virtual auto HandleMessage [[nodiscard]] (WsMessage message) const
      -> cppcoro::task<> = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_WS_MESSAGE_HANDLER_H_
