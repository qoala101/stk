#include "network_auto_parsable_ws_message.h"

#include <polymorphic_value.h>

#include <gsl/assert>
#include <type_traits>
#include <utility>

namespace stonks::network {
AutoParsableWsMessage::AutoParsableWsMessage(WsMessage message)
    : message_{std::move(message)} {}

auto AutoParsableWsMessage::operator*() -> AutoParsable {
  Expects(message_.has_value());
  auto result = AutoParsable{std::move(*message_)};

  message_.reset();
  Ensures(!message_.has_value());

  return result;
}
}  // namespace stonks::network