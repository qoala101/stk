#include "network_rest_request_builder.h"

#include <gsl/assert>
#include <string>
#include <utility>

#include "network_types.h"

namespace stonks::network {
auto RestRequestBuilder::WithMethod(Method method) -> RestRequestBuilder& {
  request_.endpoint.method = method;
  return *this;
}

auto RestRequestBuilder::WithBaseUri(std::string base_uri)
    -> RestRequestBuilder& {
  Expects(!uri_is_set_);
  request_.endpoint.uri = std::move(base_uri);
  uri_is_set_ = true;
  Ensures(uri_is_set_);
  return *this;
}

auto RestRequestBuilder::AppendUri(std::string_view uri)
    -> RestRequestBuilder& {
  Expects(uri_is_set_);

  if (!uri.starts_with("/")) {
    request_.endpoint.uri += "/";
  }

  request_.endpoint.uri += uri.data();
  Ensures(!request_.endpoint.uri.empty());
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
  request_.body.emplace(std::move(body));
  Ensures(request_.body.has_value());
  return *this;
}

auto RestRequestBuilder::Build() const& -> RestRequest {
  Expects(uri_is_set_);
  return request_;
}

auto RestRequestBuilder::Build() && -> RestRequest {
  Expects(uri_is_set_);
  auto request = RestRequest{std::move(request_)};
  request_ = {};
  Ensures(request_.IsEmpty());
  Ensures(!uri_is_set_);
  return request;
}
}  // namespace stonks::network