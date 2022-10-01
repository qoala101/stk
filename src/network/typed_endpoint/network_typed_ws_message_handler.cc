#include "network_typed_ws_message_handler.h"

#include <bits/exception.h>

#include <function2/function2.hpp>
#include <memory>
#include <utility>

#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
TypedWsMessageHandler::TypedWsMessageHandler(
    ParseTypeCheck received_message_type, cpp::NnUp<IWsMessageHandler> handler)
    : received_message_type_{[&received_message_type]() {
        Expects(!received_message_type.empty());
        return std::move(received_message_type);
      }()},
      handler_{std::move(handler)} {}

void TypedWsMessageHandler::HandleMessage(WsMessage message) const {
  try {
    received_message_type_(*message);
  } catch (const std::exception &) {
    return;
  }

  handler_->HandleMessage(std::move(message));
}
}  // namespace stonks::network