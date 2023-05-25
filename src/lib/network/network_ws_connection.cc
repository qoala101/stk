/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_ws_connection.h"

#include <coroutine>
#include <cppcoro/task.hpp>

namespace vh::network {
WsConnection::WsConnection(cpp::NnUp<IWsClient> ws_client)
    : ws_client_{std::move(ws_client)} {}

auto WsConnection::Send(WsMessage message) const -> cppcoro::task<> {
  co_await ws_client_->SendMessage(std::move(message));
}
}  // namespace vh::network