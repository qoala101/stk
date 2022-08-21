#include "network_rest_client_request_builder.h"

#include <gsl/assert>
#include <string>
#include <tuple>
#include <utility>

#include "ccutils_move_if_this_is_rvalue.h"
#include "network_types.h"

namespace stonks::network::rest_client {
void RequestBuilder::DiscardingResultImpl(auto&& t) {
  std::ignore = t.request_sender_->SendRequestAndGetResponse(
      ccutils::MoveIfThisIsRvalue<decltype(t)>(t.request_));
}

void RequestBuilder::DiscardingResult() const& { DiscardingResultImpl(*this); }

void RequestBuilder::DiscardingResult() && {
  DiscardingResultImpl(std::move(*this));
  request_ = RestRequest{};
  Ensures(request_.IsEmpty());
}

RequestBuilder::RequestBuilder(Endpoint endpoint,
                               ccutils::NnSp<IRestRequestSender> request_sender)
    : request_{std::move(endpoint)},
      request_sender_{std::move(request_sender)} {}

auto RequestBuilder::SendRequestAndGetResultImpl(auto&& t)
    -> Result::value_type {
  auto result = t.request_sender_
                    ->SendRequestAndGetResponse(
                        ccutils::MoveIfThisIsRvalue<decltype(t)>(t.request_))
                    .result;
  Expects(result);
  return std::move(*result);
}

auto RequestBuilder::WithParam(std::string key, Param value)
    -> RequestBuilder& {
  request_.params.emplace(std::move(key), std::move(value));
  Ensures(!request_.params.empty());
  return *this;
}

auto RequestBuilder::WithBody(Body::value_type body) -> RequestBuilder& {
  Expects(!request_.body.has_value());
  request_.body.emplace(std::move(body));
  Ensures(request_.body.has_value());
  return *this;
}

auto RequestBuilder::SendRequestAndGetResult() const& -> Result::value_type {
  return SendRequestAndGetResultImpl(*this);
}

auto RequestBuilder::SendRequestAndGetResult() && -> Result::value_type {
  auto result = SendRequestAndGetResultImpl(std::move(*this));
  request_ = RestRequest{};
  Ensures(request_.IsEmpty());
  return result;
}
}  // namespace stonks::network::rest_client