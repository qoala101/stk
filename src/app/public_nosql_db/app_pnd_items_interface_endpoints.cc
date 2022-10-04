#include "app_pnd_items_interface_endpoints.h"

#include <string>

#include "cpp_optional.h"
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_typed_endpoint.h"
#include "nosqldb_types.h"

namespace stonks::app::pnd::endpoints {
auto SelectItem() -> const network::TypedEndpoint& {
  static const auto endpoint = network::TypedEndpoint{
      .endpoint = {.method = network::Method::kGet, .uri = {"/SelectItem"}},
      .expected_types = {
          .params = {{"table", network::ExpectedType<nosqldb::Table>()},
                     {"key", network::ExpectedType<nosqldb::Key>()}},
          .result = network::ExpectedType<cpp::Opt<nosqldb::Item>>()}};
  return endpoint;
}

auto InsertOrUpdateItem() -> const network::TypedEndpoint& {
  static const auto endpoint = network::TypedEndpoint{
      .endpoint = {.method = network::Method::kPost,
                   .uri = {"/InsertOrUpdateItem"}},
      .expected_types = {
          .params = {{"table", network::ExpectedType<nosqldb::Table>()},
                     {"item", network::ExpectedType<nosqldb::Item>()}}}};
  return endpoint;
}
}  // namespace stonks::app::pnd::endpoints