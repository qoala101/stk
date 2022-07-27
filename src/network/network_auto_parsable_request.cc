#include "network_auto_parsable_request.h"

#include <polymorphic_value.h>

#include <gsl/assert>
#include <map>
#include <string>
#include <utility>

namespace stonks::network {
AutoParsableRestRequest::AutoParsableRestRequest(RestRequest rest_request)
    : rest_request_{std::move(rest_request)} {}

auto AutoParsableRestRequest::Param(std::string_view key) -> AutoParsable {
  const auto value = rest_request_.params.find(key.data());
  Expects(value != rest_request_.params.end());
  auto result = AutoParsable{std::move(value->second)};

  rest_request_.params.erase(value);
  Ensures(!rest_request_.params.contains(key.data()));

  return result;
}

auto AutoParsableRestRequest::Body() -> AutoParsable {
  Expects(rest_request_.body.has_value());
  auto result = AutoParsable{std::move(*rest_request_.body)};

  rest_request_.body.reset();
  Ensures(!rest_request_.body.has_value());

  return result;
}
}  // namespace stonks::network