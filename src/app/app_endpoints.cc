#include "app_endpoints.h"

#include <string>

#include "core_types.h"
#include "cpp_optional.h"
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_typed_endpoint.h"

namespace stonks::app::endpoints {
auto SelectAssets() -> const network::TypedEndpoint& {
  // static const auto kEndpoint = network::TypedEndpoint{
  //     .endpoint = {.method = network::Method::kGet, .uri =
  //     {"/SelectAssets"}},
  //     // TODO(vh): .expected_type = network::FromFunction(&App::SelectAssets,
  //     0) .expected_types = {
  //         .result = network::ExpectedType<std::vector<core::Asset>>()}};
  // return kEndpoint;
}

auto UpdateAssets() -> const network::TypedEndpoint& {
  // static const auto kEndpoint = network::TypedEndpoint{
  //     .endpoint = {.method = network::Method::kPost, .uri =
  //     {"/UpdateAssets"}}, .expected_types = {
  //         .body = network::ExpectedType<std::vector<core::Asset>>()}};
  // return kEndpoint;
}

auto SelectSymbolsWithPriceRecords() -> const network::TypedEndpoint& {
  // static const auto kEndpoint = network::TypedEndpoint{
  //     .endpoint = {.method = network::Method::kPost,
  //                  .uri = {"/InsertOrUpdateSymbolInfo"}},
  //     .expected_types = {.body = network::ExpectedType<core::SymbolInfo>()}};
  // return kEndpoint;
}

auto SelectSymbolInfo() -> const network::TypedEndpoint& {
  // static const auto kEndpoint = network::TypedEndpoint{
  //     .endpoint = {.method = network::Method::kPost,
  //                  .uri = {"/InsertOrUpdateSymbolInfo"}},
  //     .expected_types = {.body = network::ExpectedType<core::SymbolInfo>()}};
  // return kEndpoint;
}

auto SelectSymbolsInfo() -> const network::TypedEndpoint& {
  // static const auto kEndpoint = network::TypedEndpoint{
  //     .endpoint = {.method = network::Method::kPost,
  //                  .uri = {"/InsertOrUpdateSymbolInfo"}},
  //     .expected_types = {.body = network::ExpectedType<core::SymbolInfo>()}};
  // return kEndpoint;
}

auto UpdateSymbolsInfo() -> const network::TypedEndpoint& {
  // static const auto kEndpoint = network::TypedEndpoint{
  //     .endpoint = {.method = network::Method::kPost,
  //                  .uri = {"/InsertOrUpdateSymbolInfo"}},
  //     .expected_types = {.body = network::ExpectedType<core::SymbolInfo>()}};
  // return kEndpoint;
}

auto SelectSymbolPriceRecords() -> const network::TypedEndpoint& {
  // static const auto kEndpoint = network::TypedEndpoint{
  //     .endpoint = {.method = network::Method::kPost,
  //                  .uri = {"/InsertOrUpdateSymbolInfo"}},
  //     .expected_types = {.body = network::ExpectedType<core::SymbolInfo>()}};
  // return kEndpoint;
}

auto InsertSymbolPriceRecord() -> const network::TypedEndpoint& {
  // static const auto kEndpoint = network::TypedEndpoint{
  //     .endpoint = {.method = network::Method::kPost,
  //                  .uri = {"/InsertSymbolPriceRecord"}},
  //     .expected_types = {.body =
  //                            network::ExpectedType<core::SymbolPriceRecord>()}};
  // return kEndpoint;
}

auto DeleteSymbolPriceRecords() -> const network::TypedEndpoint& {
  // static const auto kEndpoint = network::TypedEndpoint{
  //     .endpoint = {.method = network::Method::kPost,
  //                  .uri = {"/InsertOrUpdateSymbolInfo"}},
  //     .expected_types = {.body = network::ExpectedType<core::SymbolInfo>()}};
  // return kEndpoint;
}
}  // namespace stonks::app::endpoints