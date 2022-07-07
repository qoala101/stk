#include "network_rest_request_builder.h"

#include <gsl/assert>
#include <stdexcept>

#include "network_types.h"

namespace stonks::network {
auto RestRequestBuilder::WithMethod(Method method) -> RestRequestBuilder& {
  method_ = method;
  return *this;
}

auto RestRequestBuilder::WithBaseUri(std::string_view base_uri)
    -> RestRequestBuilder& {
  Expects(uri_parts_.empty());
  uri_parts_.emplace_back(base_uri.data());
  return *this;
}

auto RestRequestBuilder::AppendUri(std::string_view uri)
    -> RestRequestBuilder& {
  Expects(!uri_parts_.empty());
  uri_parts_.emplace_back(uri.data());
  return *this;
}

auto RestRequestBuilder::AddParam(std::string_view key, std::string_view value)
    -> RestRequestBuilder& {
  params_[key.data()] = value.data();
  return *this;
}

auto RestRequestBuilder::AddParam(std::string_view key,
                                  std::chrono::milliseconds value)
    -> RestRequestBuilder& {
  return AddParam(key, value.count());
}

auto RestRequestBuilder::AddHeader(std::string_view key, std::string_view value)
    -> RestRequestBuilder& {
  headers_[key.data()] = value.data();
  return *this;
}

auto RestRequestBuilder::WithBody(Json body) -> RestRequestBuilder& {
  Expects(!body_.has_value());
  body_.emplace(std::move(body));
  return *this;
}

auto RestRequestBuilder::Build() const -> std::pair<Endpoint, RestRequestData> {
  throw std::runtime_error{"NOT IMPLEMENTED"};
}
}  // namespace stonks::network