#include "network_rest_client_request_builder.h"

#include <polymorphic_value.h>

#include <gsl/assert>
#include <string>
#include <tuple>
#include <utility>

#include "cpp_move_if.h"
#include "network_types.h"

namespace stonks::network::rest_client {
template <cpp::DecaysTo<RequestBuilder> This>
void RequestBuilder::DiscardingResultImpl(This& t) {
  Expects(t.request_.has_value());
  std::ignore = t.request_sender_->SendRequestAndGetResponse(
      cpp::MoveIfNotConst<This>(*t.request_));
}

void RequestBuilder::DiscardingResult() const { DiscardingResultImpl(*this); }

void RequestBuilder::DiscardingResult() {
  DiscardingResultImpl(*this);
  request_.reset();
  Ensures(!request_.has_value());
}

RequestBuilder::RequestBuilder(Endpoint endpoint,
                               cpp::NnUp<IRestRequestSender> request_sender)
    : request_{{.endpoint = std::move(endpoint)}},
      request_sender_{std::move(request_sender)} {}

template <cpp::DecaysTo<RequestBuilder> This>
auto RequestBuilder::SendRequestAndGetResultImpl(This& t) {
  Expects(t.request_.has_value());
  auto result =
      t.request_sender_
          ->SendRequestAndGetResponse(cpp::MoveIfNotConst<This>(*t.request_))
          .result;
  Expects(result.has_value());
  return std::move(*result);
}

auto RequestBuilder::WithParam(std::string key, Param value)
    -> RequestBuilder& {
  Expects(request_.has_value());
  request_->params.emplace(std::move(key), std::move(value));
  Ensures(!request_->params.empty());
  return *this;
}

auto RequestBuilder::WithBody(Body::value_type body) -> RequestBuilder& {
  Expects(request_.has_value());
  Expects(!request_->body.has_value());
  request_->body = std::move(body);
  Ensures(request_->body.has_value());
  return *this;
}

auto RequestBuilder::SendRequestAndGetResult() const -> Result::value_type {
  return SendRequestAndGetResultImpl(*this);
}

auto RequestBuilder::SendRequestAndGetResult() -> Result::value_type {
  auto result = SendRequestAndGetResultImpl(*this);
  request_.reset();
  Ensures(!request_.has_value());
  return result;
}
}  // namespace stonks::network::rest_client