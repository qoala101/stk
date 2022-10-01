#include "network_web_socket_sender.h"

#include <memory>

#include "network_types.h"

namespace stonks::network {
WebSocketSender::WebSocketSender(cpp::NnUp<IWebSocketClient> web_socket_client)
    : web_socket_client_{std::move(web_socket_client)} {}

void WebSocketSender::Send(WsMessage message) const {
  web_socket_client_->SendMessage(std::move(message));
}
}  // namespace stonks::network