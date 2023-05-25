/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_auto_parsable_ws_message.h"

#include <gsl/assert>
#include <utility>

namespace vh::network {
AutoParsableWsMessage::AutoParsableWsMessage(WsMessage message)
    : message_{std::move(message)} {}

auto AutoParsableWsMessage::operator*() -> AutoParsable {
  Expects(message_.has_value());
  auto result = AutoParsable{std::move(*message_)};

  message_.reset();
  Ensures(!message_.has_value());

  return result;
}
}  // namespace vh::network