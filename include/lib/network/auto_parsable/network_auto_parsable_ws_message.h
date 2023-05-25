/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_AUTO_PARSABLE_WS_MESSAGE_H_
#define VH_NETWORK_AUTO_PARSABLE_WS_MESSAGE_H_

#include "cpp_optional.h"
#include "network_auto_parsable.h"
#include "network_ws_types.h"

namespace vh::network {
/**
 * @brief Wraps raw web socket message providing convenient auto-parsable API
 * for it.
 */
class AutoParsableWsMessage {
 public:
  explicit AutoParsableWsMessage(WsMessage message);

  /**
   * @brief Gives message content by moving it into result.
   * @remark Should not be called more than once.
   */
  auto operator* [[nodiscard]] () -> AutoParsable;

 private:
  cpp::Opt<WsMessage> message_;
};
}  // namespace vh::network

#endif  // VH_NETWORK_AUTO_PARSABLE_WS_MESSAGE_H_
