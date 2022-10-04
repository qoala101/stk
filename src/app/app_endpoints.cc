#include "app_endpoints.h"

#include <string>

#include "core_types.h"
#include "cpp_optional.h"
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_typed_endpoint.h"

namespace stonks::app::endpoints {
auto InsertOrUpdateSymbolInfo() -> const network::TypedEndpoint& {
  static const auto kEndpoint = network::TypedEndpoint{
      .endpoint = {.method = network::Method::kPost,
                   .uri = {"/InsertOrUpdateSymbolInfo"}},
      .expected_types = {.body = network::ExpectedType<core::SymbolInfo>()}};
  return kEndpoint;
}

auto InsertSymbolPriceRecord() -> const network::TypedEndpoint& {
  static const auto kEndpoint = network::TypedEndpoint{
      .endpoint = {.method = network::Method::kPost,
                   .uri = {"/InsertSymbolPriceRecord"}},
      .expected_types = {.body =
                             network::ExpectedType<core::SymbolPriceRecord>()}};
  return kEndpoint;
}
}  // namespace stonks::app::endpoints