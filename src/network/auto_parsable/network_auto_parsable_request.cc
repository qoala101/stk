#include "network_auto_parsable_request.h"

#include <gsl/assert>
#include <map>
#include <string>
#include <utility>

namespace stonks::network {
AutoParsableRestRequest::AutoParsableRestRequest(RestRequest rest_request)
    : params_{std::move(rest_request.params)},
      body_{std::move(rest_request.body)} {
  Ensures(body_.has_value());
}

auto AutoParsableRestRequest::Param(std::string_view key) -> AutoParsable {
  const auto value = params_.find(key.data());
  Expects(value != params_.end());
  auto result = AutoParsable{std::move(value->second)};

  params_.erase(value);
  Ensures(!params_.contains(key.data()));

  return result;
}

auto AutoParsableRestRequest::Body() -> AutoParsable {
  Expects(body_.has_value());
  auto result = AutoParsable{std::move(*body_)};

  body_.reset();
  Ensures(!body_.has_value());

  return result;
}
}  // namespace stonks::network