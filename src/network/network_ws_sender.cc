#include "network_ws_sender.h"

#include <memory>

#include "network_types.h"

namespace stonks::network {
WsSender::WsSender(cpp::NnUp<IWsClient> ws_client)
    : ws_client_{std::move(ws_client)} {}

void WsSender::Send(WsMessage message) const {
  ws_client_->SendMessage(std::move(message));
}
}  // namespace stonks::network