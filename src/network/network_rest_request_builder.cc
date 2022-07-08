#include "network_rest_request_builder.h"

#include <gsl/assert>
#include <range/v3/functional/identity.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <type_traits>
#include <utility>

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
  auto endpoint =
      Endpoint{.method = method_, .uri = AccumulateUriParts(uri_parts_)};
  auto data =
      RestRequestData{.params = params_, .headers = headers_, .body = body_};
  return std::make_pair(std::move(endpoint), std::move(data));
}
}  // namespace stonks::network