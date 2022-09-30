#include "network_auto_parsable_web_socket_handler.h"

#include <utility>

namespace stonks::network {
void AutoParsableWebSocketHandler::HandleMessage(WsMessage message) const {
  handler_.TODO(std::move(message));
}
}  // namespace stonks::network