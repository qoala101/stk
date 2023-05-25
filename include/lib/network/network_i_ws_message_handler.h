/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_I_WS_MESSAGE_HANDLER_H_
#define VH_NETWORK_I_WS_MESSAGE_HANDLER_H_

#include <cppcoro/task.hpp>

#include "network_ws_types.h"

namespace vh::network {
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
  virtual auto HandleMessage [[nodiscard]] (WsMessage message)
  -> cppcoro::task<> = 0;
};
}  // namespace vh::network

#endif  // VH_NETWORK_I_WS_MESSAGE_HANDLER_H_
