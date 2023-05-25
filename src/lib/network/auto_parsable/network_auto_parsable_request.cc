/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_auto_parsable_request.h"

#include <absl/container/flat_hash_map.h>
#include <absl/meta/type_traits.h>

#include <gsl/assert>
#include <utility>

#include "network_json_base_conversions.h"

namespace vh::network {
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
}  // namespace vh::network