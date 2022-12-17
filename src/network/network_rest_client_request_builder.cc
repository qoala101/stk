#include "network_rest_client_request_builder.h"

#include <polymorphic_value.h>

#include <coroutine>
#include <cppcoro/task.hpp>
#include <gsl/assert>
#include <string>
#include <utility>

#include "cpp_move_if.h"
#include "cpp_polymorphic_value.h"
#include "cpp_swap_reset.h"
#include "network_types.h"

namespace stonks::network::rest_client {
template <cpp::This<RequestBuilder> This>
auto RequestBuilder::DiscardingResultImpl(This& t) -> cppcoro::task<> {
  std::ignore = co_await t.SendRequestAndGetResult();
}

auto RequestBuilder::DiscardingResult() const -> cppcoro::task<> {
  co_await DiscardingResultImpl(*this);
}

auto RequestBuilder::DiscardingResult() -> cppcoro::task<> {
  co_await DiscardingResultImpl(*this);
}

RequestBuilder::RequestBuilder(Endpoint endpoint,
                               cpp::NnUp<IRestRequestSender> request_sender)
    : endpoint_{std::move(endpoint)},
      request_sender_{std::move(request_sender)} {}

template <Parsable T, cpp::This<RequestBuilder> This>
auto RequestBuilder::SendRequestAndGetResultImpl(This& t) -> cppcoro::task<T> {
  auto request = RestRequest{.endpoint = t.endpoint_,
                             .params = cpp::MoveIfNotConst<This>(t.params_),
                             .headers = cpp::MoveIfNotConst<This>(t.headers_),
                             .body = t.body_.has_value()
                                         ? cpp::MoveIfNotConst<This>(*t.body_)
                                         : CreateNullJson()};
  auto response =
      co_await t.request_sender_->SendRequestAndGetResponse(std::move(request));
  co_return std::move(response.result);
}

auto RequestBuilder::WithParam(std::string_view key, Param value)
    -> RequestBuilder& {
  Expects(!params_.contains(key.data()));
  params_.emplace(key, std::move(value));
  Ensures(params_.contains(key.data()));
  return *this;
}

auto RequestBuilder::WithBody(Body body) -> RequestBuilder& {
  Expects(!body_.has_value());
  body_ = std::move(body);
  Ensures(body_.has_value());
  return *this;
}

auto RequestBuilder::SendRequestAndGetResult() const -> cppcoro::task<Result> {
  co_return co_await SendRequestAndGetResultImpl<Result>(*this);
}

auto RequestBuilder::SendRequestAndGetResult() -> cppcoro::task<Result> {
  auto result = co_await SendRequestAndGetResultImpl<Result>(*this);

  cpp::SwapReset(*this, RequestBuilder{{}, std::move(request_sender_)});
  Ensures(params_.empty());
  Ensures(headers_.empty());
  Ensures(!body_.has_value());

  co_return result;
}
}  // namespace stonks::network::rest_client