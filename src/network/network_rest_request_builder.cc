#include "network_rest_request_builder.h"

#include <polymorphic_value.h>

#include <gsl/assert>
#include <not_null.hpp>
#include <string>
#include <utility>

#include "cpp_polymorphic_value.h"
#include "cpp_typed_struct.h"
#include "network_types.h"

namespace stonks::network {
RestRequestBuilder::RestRequestBuilder() : request_{std::in_place} {}

auto RestRequestBuilder::WithMethod(Method method) -> RestRequestBuilder& {
  Expects(request_.has_value());
  request_->endpoint.method = method;
  return *this;
}

auto RestRequestBuilder::WithBaseUri(Uri base_uri) -> RestRequestBuilder& {
  Expects(!uri_is_set_);
  Expects(request_.has_value());
  request_->endpoint.uri = std::move(base_uri);
  uri_is_set_ = true;
  Ensures(uri_is_set_);
  return *this;
}

auto RestRequestBuilder::AppendUri(Uri uri) -> RestRequestBuilder& {
  Expects(uri_is_set_);
  Expects(request_.has_value());

  if (!uri->starts_with("/")) {
    *request_->endpoint.uri += "/";
  }

  *request_->endpoint.uri += std::move(uri);
  Ensures(!request_->endpoint.uri->empty());
  return *this;
}

auto RestRequestBuilder::AddParam(std::string key, Param value)
    -> RestRequestBuilder& {
  Expects(request_.has_value());
  request_->params.emplace(std::move(key), std::move(value));
  Ensures(!request_->params.empty());
  return *this;
}

auto RestRequestBuilder::AddHeader(std::string key, std::string value)
    -> RestRequestBuilder& {
  Expects(request_.has_value());
  request_->headers.emplace(std::move(key), std::move(value));
  Ensures(!request_->headers.empty());
  return *this;
}

auto RestRequestBuilder::WithBody(Body::value_type body)
    -> RestRequestBuilder& {
  Expects(request_.has_value());
  Expects(!request_->body.has_value());
  request_->body = std::move(body);
  Ensures(request_->body.has_value());
  return *this;
}

auto RestRequestBuilder::Build() const -> RestRequest {
  Expects(uri_is_set_);
  Expects(request_.has_value());
  return *request_;
}

auto RestRequestBuilder::Build() -> RestRequest {
  Expects(uri_is_set_);
  Expects(request_.has_value());
  auto request = RestRequest{std::move(*request_)};
  request_.reset();
  uri_is_set_ = false;
  Ensures(!request_.has_value());
  Ensures(!uri_is_set_);
  return request;
}
}  // namespace stonks::network