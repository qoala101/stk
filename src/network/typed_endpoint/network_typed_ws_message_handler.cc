#include "network_typed_ws_message_handler.h"

#include <bits/exception.h>

#include <function2/function2.hpp>
#include <gsl/assert>
#include <memory>
#include <utility>

#include "network_typed_endpoint.h"

namespace stonks::network {
TypedWsMessageHandler::TypedWsMessageHandler(
    ParseTypeCheck received_message_type, cpp::NnUp<IWsMessageHandler> handler)
    : received_message_type_{[&received_message_type]() {
        Expects(!received_message_type.empty());
        return std::move(received_message_type);
      }()},
      handler_{std::move(handler)} {}

auto TypedWsMessageHandler::HandleMessage(WsMessage message) const
    -> cppcoro::task<> {
  try {
    received_message_type_(*message);
  } catch (const std::exception &) {
    co_return;
  }

  co_await handler_->HandleMessage(std::move(message));
}
}  // namespace stonks::network