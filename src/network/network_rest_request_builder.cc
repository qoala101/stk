#include "network_rest_request_builder.h"

#include <gsl/assert>
#include <string>
#include <type_traits>
#include <utility>

#include "network_types.h"

namespace stonks::network {
auto RestRequestBuilder::WithMethod(Method method) -> RestRequestBuilder& {
  request_.endpoint.method = method;
  return *this;
}

auto RestRequestBuilder::WithBaseUri(Uri base_uri) -> RestRequestBuilder& {
  Expects(!uri_is_set_);
  request_.endpoint.uri = std::move(base_uri);
  uri_is_set_ = true;
  Ensures(uri_is_set_);
  return *this;
}

auto RestRequestBuilder::AppendUri(Uri uri) -> RestRequestBuilder& {
  Expects(uri_is_set_);

  if (!uri.value.starts_with("/")) {
    request_.endpoint.uri.value += "/";
  }

  request_.endpoint.uri.value += std::move(uri);
  Ensures(!request_.endpoint.uri.value.empty());
  return *this;
}

auto RestRequestBuilder::AddParam(std::string key, Param value)
    -> RestRequestBuilder& {
  request_.params.emplace(std::move(key), std::move(value));
  Ensures(!request_.params.empty());
  return *this;
}

auto RestRequestBuilder::AddHeader(std::string key, std::string value)
    -> RestRequestBuilder& {
  request_.headers.emplace(std::move(key), std::move(value));
  Ensures(!request_.headers.empty());
  return *this;
}

auto RestRequestBuilder::WithBody(Body::value_type body)
    -> RestRequestBuilder& {
  Expects(!request_.body.has_value());
  request_.body = std::move(body);
  Ensures(request_.body.has_value());
  return *this;
}

auto RestRequestBuilder::Build() const -> RestRequest {
  Expects(uri_is_set_);
  return request_;
}

auto RestRequestBuilder::Build() -> RestRequest {
  Expects(uri_is_set_);
  auto request = RestRequest{std::move(request_)};
  request_ = {};
  uri_is_set_ = false;
  Ensures(request_.IsEmpty());
  Ensures(!uri_is_set_);
  return request;
}
}  // namespace stonks::network