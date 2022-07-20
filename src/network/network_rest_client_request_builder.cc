#include "network_rest_client_request_builder.h"

#include <gsl/assert>
#include <tuple>
#include <utility>

#include "network_types.h"

namespace stonks::network::rest_client {
void RequestBuilder::DiscardingResult() const {
  std::ignore = request_sender_->SendRequestAndGetResponse(endpoint_, data_);
}

RequestBuilder::RequestBuilder(
    Endpoint endpoint,
    cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender)
    : endpoint_{std::move(endpoint)},
      request_sender_{std::move(request_sender)} {}

auto RequestBuilder::WithParam(std::string_view key, Param value)
    -> RequestBuilder& {
  data_.params.emplace(key.data(), std::move(value));
  Ensures(!data_.params.empty());
  return *this;
}

auto RequestBuilder::WithBody(Body::value_type body) -> RequestBuilder& {
  Expects(!data_.body.has_value());
  data_.body.emplace(std::move(body));
  Ensures(data_.body.has_value());
  return *this;
}

auto RequestBuilder::SendRequestAndGetResponse() const -> Result::value_type {
  auto result =
      request_sender_->SendRequestAndGetResponse(endpoint_, data_).second;
  Expects(result);
  return *result;
}
}  // namespace stonks::network::rest_client