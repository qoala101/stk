/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_typed_ws_message_handler.h"

#include <coroutine>
#include <exception>
#include <function2/function2.hpp>
#include <gsl/assert>
#include <not_null.hpp>
#include <utility>

#include "network_typed_endpoint.h"

namespace vh::network {
TypedWsMessageHandler::TypedWsMessageHandler(
    ParseTypeCheck received_message_type, cpp::NnUp<IWsMessageHandler> handler)
    : received_message_type_{(Expects(!received_message_type.empty()),
                              std::move(received_message_type))},
      handler_{std::move(handler)} {}

auto TypedWsMessageHandler::HandleMessage(WsMessage message)
    -> cppcoro::task<> {
  try {
    received_message_type_(*message);
  } catch (const std::exception &) {
    co_return;
  }

  co_await handler_->HandleMessage(std::move(message));
}
}  // namespace vh::network