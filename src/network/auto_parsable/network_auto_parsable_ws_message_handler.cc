#include "network_auto_parsable_ws_message_handler.h"

#include <cppcoro/task.hpp>
#include <utility>

namespace stonks::network {
auto AutoParsableWsMessageHandler::HandleMessage(WsMessage message) const
    -> cppcoro::task<> {
  co_await handler_(std::move(message));
}
}  // namespace stonks::network