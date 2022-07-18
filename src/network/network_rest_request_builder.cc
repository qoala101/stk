#include "network_rest_request_builder.h"

#include <gsl/assert>
#include <range/v3/functional/identity.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>
#include <vector>

#include "network_types.h"

namespace stonks::network {
namespace {
auto AccumulateUriParts(const std::vector<std::string>& uri_parts)
    -> std::string {
  if (uri_parts.empty()) {
    return {};
  }

  return ranges::accumulate(uri_parts | ranges::views::drop(1),
                            uri_parts.front(),
                            [](const auto& result, const auto& uri_part) {
                              return result + "/" + uri_part;
                            });
};
}  // namespace

auto RestRequestBuilder::WithMethod(Method method) -> RestRequestBuilder& {
  method_ = method;
  return *this;
}

auto RestRequestBuilder::WithBaseUri(std::string_view base_uri)
    -> RestRequestBuilder& {
  Expects(!uri_.has_value());
  uri_.emplace(base_uri.data());
  Ensures(uri_.has_value());
  return *this;
}

auto RestRequestBuilder::AppendUri(std::string_view uri)
    -> RestRequestBuilder& {
  Expects(uri_.has_value());
  (*uri_) += std::string{"/"} + uri.data();
  return *this;
}

auto RestRequestBuilder::AddParam(std::string_view key, Param value)
    -> RestRequestBuilder& {
  params_.emplace(key.data(), std::move(value));
  return *this;
}

auto RestRequestBuilder::AddHeader(std::string_view key, std::string_view value)
    -> RestRequestBuilder& {
  headers_.emplace(key.data(), value.data());
  Ensures(!headers_.empty());
  return *this;
}

auto RestRequestBuilder::WithBody(Body body) -> RestRequestBuilder& {
  Expects(!body_);
  body_.emplace(std::move(body));
  Ensures(body_);
  return *this;
}

auto RestRequestBuilder::Build()
    const& -> std::pair<Endpoint, RestRequestData> {
  Expects(uri_.has_value());
  auto endpoint = Endpoint{.method = method_, .uri = *uri_};
  auto data =
      RestRequestData{.params = params_, .headers = headers_, .body = body_};
  return std::make_pair(std::move(endpoint), std::move(data));
}

auto RestRequestBuilder::Build() && -> std::pair<Endpoint, RestRequestData> {
  Expects(uri_.has_value());
  auto endpoint = Endpoint{.method = method_, .uri = std::move(*uri_)};
  auto data = RestRequestData{.params = std::move(params_),
                              .headers = std::move(headers_),
                              .body = std::move(body_)};
  return std::make_pair(std::move(endpoint), std::move(data));
}
}  // namespace stonks::network