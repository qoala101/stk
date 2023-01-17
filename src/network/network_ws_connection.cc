#include "network_ws_connection.h"

#include <coroutine>
#include <cppcoro/task.hpp>

namespace stonks::network {
WsConnection::WsConnection(cpp::meta::PrivateTo<WsClientBuilder> /*unused*/,
                           cpp::NnUp<IWsClient> ws_client)
    : ws_client_{std::move(ws_client)} {}

auto WsConnection::Send(WsMessage message) const -> cppcoro::task<> {
  co_await ws_client_->SendMessage(std::move(message));
}
}  // namespace stonks::network