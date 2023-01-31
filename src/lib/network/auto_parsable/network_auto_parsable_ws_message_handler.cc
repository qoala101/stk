#include "network_auto_parsable_ws_message_handler.h"

#include <coroutine>
#include <cppcoro/task.hpp>
#include <utility>

namespace stonks::network {
auto AutoParsableWsMessageHandler::HandleMessage(WsMessage message)
    -> cppcoro::task<> {
  co_await handler_(std::move(message));
}
}  // namespace stonks::network