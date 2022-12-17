#include "network_rest_request_builder.h"

#include <polymorphic_value.h>

#include <gsl/assert>
#include <not_null.hpp>
#include <string>
#include <utility>

#include "cpp_polymorphic_value.h"
#include "cpp_swap_reset.h"
#include "cpp_typed_struct.h"
#include "network_json_base_conversions.h"
#include "network_types.h"

namespace stonks::network {
auto RestRequestBuilder::WithMethod(Method method) -> RestRequestBuilder& {
  Expects(!method_.has_value());
  method_ = method;
  Ensures(method_.has_value());
  return *this;
}

auto RestRequestBuilder::WithBaseUri(Uri base_uri) -> RestRequestBuilder& {
  Expects(!uri_.has_value());
  uri_ = std::move(base_uri);
  Ensures(uri_.has_value());
  return *this;
}

auto RestRequestBuilder::AppendUri(const Uri& uri) -> RestRequestBuilder& {
  Expects(uri_.has_value());

  if (!uri->starts_with("/")) {
    **uri_ += "/";
  }

  **uri_ += uri;

  Ensures(!(*uri_)->empty());
  return *this;
}

auto RestRequestBuilder::AddParam(std::string_view key, Param value)
    -> RestRequestBuilder& {
  Expects(!params_.contains(key.data()));
  params_.emplace(key, std::move(value));
  Ensures(params_.contains(key.data()));
  return *this;
}

auto RestRequestBuilder::AddHeader(std::string_view key, std::string value)
    -> RestRequestBuilder& {
  Expects(!headers_.contains(key.data()));
  headers_.emplace(key, std::move(value));
  Ensures(headers_.contains(key.data()));
  return *this;
}

auto RestRequestBuilder::WithBody(Body body) -> RestRequestBuilder& {
  Expects(!body_.has_value());
  body_ = std::move(body);
  Ensures(body_.has_value());
  return *this;
}

auto RestRequestBuilder::Build() const -> RestRequest {
  Expects(uri_.has_value());
  return RestRequest{
      .endpoint = {.method = method_.value_or(Method::kGet), .uri = *uri_},
      .params = params_,
      .headers = headers_,
      .body = body_.has_value() ? *body_ : CreateNullJson()};
}

auto RestRequestBuilder::Build() -> RestRequest {
  Expects(uri_.has_value());

  auto request = RestRequest{
      .endpoint = {.method = method_.value_or(Method::kGet),
                   .uri = std::move(*uri_)},
      .params = std::move(params_),
      .headers = std::move(headers_),
      .body = body_.has_value() ? std::move(*body_) : CreateNullJson()};

  cpp::SwapReset(*this);
  Ensures(!method_.has_value());
  Ensures(!uri_.has_value());
  Ensures(params_.empty());
  Ensures(headers_.empty());
  Ensures(!body_.has_value());
  return request;
}
}  // namespace stonks::network