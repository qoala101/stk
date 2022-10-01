#include "network_auto_parsable_ws_message_handler.h"

#include <utility>

namespace stonks::network {
void AutoParsableWsMessageHandler::HandleMessage(WsMessage message) const {
  handler_(std::move(message));
}
}  // namespace stonks::network