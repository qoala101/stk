#include "network_auto_parsable_request.h"

#include <gsl/assert>
#include <map>
#include <string>
#include <utility>

#include "ccutils_move_if_this_is_rvalue.h"

namespace stonks::network {
AutoParsableRestRequest::AutoParsableRestRequest(RestRequest rest_request)
    : rest_request_{std::move(rest_request)} {}

auto AutoParsableRestRequest::ParamImpl(auto&& t, std::string_view key)
    -> AutoParsable {
  auto value = t.rest_request_.params.find(key.data());
  Expects(value != t.rest_request_.params.end());
  return AutoParsable{ccutils::MoveIfThisIsRvalue<decltype(t)>(value->second)};
}

auto AutoParsableRestRequest::Param(
    std::string_view key) const& -> AutoParsable {
  return ParamImpl(*this, key);
}

auto AutoParsableRestRequest::Param(std::string_view key) && -> AutoParsable {
  return ParamImpl(std::move(*this), key);
}

auto AutoParsableRestRequest::BodyImpl(auto&& t) -> AutoParsable {
  Expects(t.rest_request_.body.has_value());
  return AutoParsable{
      ccutils::MoveIfThisIsRvalue<decltype(t)>(*t.rest_request_.body)};
}

auto AutoParsableRestRequest::Body() const& -> AutoParsable {
  return BodyImpl(*this);
}

auto AutoParsableRestRequest::Body() && -> AutoParsable {
  return BodyImpl(std::move(*this));
}
}  // namespace stonks::network