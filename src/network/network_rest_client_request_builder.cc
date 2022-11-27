#include "network_rest_client_request_builder.h"

#include <polymorphic_value.h>

#include <cppcoro/task.hpp>
#include <gsl/assert>
#include <string>
#include <tuple>
#include <utility>

#include "cpp_move_if.h"
#include "network_types.h"

namespace stonks::network::rest_client {
template <cpp::This<RequestBuilder> This>
auto RequestBuilder::DiscardingResultImpl(This& t) -> cppcoro::task<> {
  Expects(t.request_.has_value());
  co_await t.request_sender_->SendRequestAndGetResponse(
      cpp::MoveIfNotConst<This>(*t.request_));
}

auto RequestBuilder::DiscardingResult() const -> cppcoro::task<> {
  co_await DiscardingResultImpl(*this);
}

auto RequestBuilder::DiscardingResult() -> cppcoro::task<> {
  co_await DiscardingResultImpl(*this);
  request_.reset();
  Ensures(!request_.has_value());
}

RequestBuilder::RequestBuilder(Endpoint endpoint,
                               cpp::NnUp<IRestRequestSender> request_sender)
    : request_{{.endpoint = std::move(endpoint)}},
      request_sender_{std::move(request_sender)} {}

template <Parsable T, cpp::This<RequestBuilder> This>
auto RequestBuilder::SendRequestAndGetResultImpl(This& t) -> cppcoro::task<T> {
  Expects(t.request_.has_value());
  auto result = (co_await t.request_sender_->SendRequestAndGetResponse(
                     cpp::MoveIfNotConst<This>(*t.request_)))
                    .result;
  Expects(result.has_value());
  co_return std::move(*result);
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

auto RequestBuilder::SendRequestAndGetResult() const
    -> cppcoro::task<Result::value_type> {
  co_return co_await SendRequestAndGetResultImpl<Result::value_type>(*this);
}

auto RequestBuilder::SendRequestAndGetResult()
    -> cppcoro::task<Result::value_type> {
  auto result = co_await SendRequestAndGetResultImpl<Result::value_type>(*this);
  request_.reset();
  Ensures(!request_.has_value());
  co_return result;
}
}  // namespace stonks::network::rest_client