#include "network_rest_client_request_builder.h"

#include <gsl/assert>
#include <string>
#include <tuple>
#include <utility>

#include "network_types.h"

namespace stonks::network::rest_client {
void RequestBuilder::DiscardingResult() const& {
  std::ignore = request_sender_->SendRequestAndGetResponse(request_);
}

void RequestBuilder::DiscardingResult() && {
  std::ignore = request_sender_->SendRequestAndGetResponse(std::move(request_));
}

RequestBuilder::RequestBuilder(
    Endpoint endpoint,
    cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender)
    : request_{std::move(endpoint)},
      request_sender_{std::move(request_sender)} {}

auto RequestBuilder::WithParam(std::string_view key, Param value)
    -> RequestBuilder& {
  request_.params.emplace(key.data(), std::move(value));
  Ensures(!request_.params.empty());
  return *this;
}

auto RequestBuilder::WithBody(Body::value_type body) -> RequestBuilder& {
  Expects(!request_.body.has_value());
  request_.body.emplace(std::move(body));
  Ensures(request_.body.has_value());
  return *this;
}

auto RequestBuilder::SendRequestAndGetResult() const -> Result::value_type {
  auto result = request_sender_->SendRequestAndGetResponse(request_).result;
  Expects(result);
  return *result;
}

auto RequestBuilder::SendRequestAndGetResult() -> Result::value_type {
  auto result =
      request_sender_->SendRequestAndGetResponse(std::move(request_)).result;
  Expects(result);
  return *result;
}
}  // namespace stonks::network::rest_client