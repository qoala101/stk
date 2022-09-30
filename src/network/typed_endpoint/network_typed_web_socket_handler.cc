#include "network_typed_web_socket_handler.h"

#include <bits/exception.h>

#include <function2/function2.hpp>
#include <memory>
#include <utility>

#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
TypedWebSocketHandler::TypedWebSocketHandler(
    ParseTypeCheck message_type, cpp::NnSp<IWebSocketHandler> handler)
    : received_message_type_{std::move(message_type)},
      handler_{std::move(handler)} {}

void TypedWebSocketHandler::HandleMessage(WsMessage message) const {
  try {
    received_message_type_(*message);
  } catch (const std::exception &) {
    return;
  }

  handler_->HandleMessage(std::move(message));
}
}  // namespace stonks::network