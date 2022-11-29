#include "network_ws_connection.h"

#include <cppcoro/task.hpp>
#include <memory>

namespace stonks::network {
WsConnection::WsConnection(cpp::NnUp<IWsClient> ws_client)
    : ws_client_{std::move(ws_client)} {}

auto WsConnection::Send(WsMessage message) const -> cppcoro::task<> {
  co_await ws_client_->SendMessage(std::move(message));
}
}  // namespace stonks::network