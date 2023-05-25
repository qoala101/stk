/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_auto_parsable_ws_message_handler.h"

#include <coroutine>
#include <cppcoro/task.hpp>
#include <utility>

namespace vh::network {
auto AutoParsableWsMessageHandler::HandleMessage(WsMessage message)
    -> cppcoro::task<> {
  co_await handler_(std::move(message));
}
}  // namespace vh::network