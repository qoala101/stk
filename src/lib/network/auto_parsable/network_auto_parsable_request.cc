#include "network_auto_parsable_request.h"

#include <gsl/assert>
#include <map>
#include <string>
#include <utility>

#include "network_json_base_conversions.h"

namespace stonks::network {
AutoParsableRestRequest::AutoParsableRestRequest(RestRequest rest_request)
    : params_{std::move(rest_request.params)},
      body_{std::move(rest_request.body)} {
  Ensures(body_.has_value());
}

auto AutoParsableRestRequest::Param(std::string_view key) -> AutoParsable {
  const auto value = params_.find(key.data());
  const auto request_has_param = value != params_.end();

  auto json = request_has_param ? std::move(value->second) : CreateNullJson();
  auto result = AutoParsable{std::move(json)};

  if (request_has_param) {
    params_.erase(value);
  }

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