#include "network_ws_connection.h"

#include <memory>

namespace stonks::network {
WsConnection::WsConnection(cpp::NnUp<IWsClient> ws_client)
    : ws_client_{std::move(ws_client)} {}

void WsConnection::Send(WsMessage message) const {
  ws_client_->SendMessage(std::move(message));
}
}  // namespace stonks::network