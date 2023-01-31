#ifndef STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_WS_MESSAGE_H_
#define STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_WS_MESSAGE_H_

#include "cpp_optional.h"
#include "network_auto_parsable.h"
#include "network_ws_types.h"

namespace stonks::network {
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
}  // namespace stonks::network

#endif  // STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_WS_MESSAGE_H_
