#include "app_dt_stec_app_endpoints.h"

#include <string>

#include "app_dt_stec_types.h"
#include "cpp_optional.h"
#include "network_enums.h"
#include "network_typed_endpoint.h"

namespace stonks::app::dt::stec::endpoints {
auto GetAveragePrice() -> const network::TypedEndpoint& {
  static const auto endpoint = network::TypedEndpoint{
      .endpoint = {.method = network::Method::kGet,
                   .uri = {"/GetAveragePrice"}},
      .expected_types = {
          .params = {{"symbol", network::ExpectedType<Symbol>()}},
          .result = network::ExpectedType<Price>()}};
  return endpoint;
}
}  // namespace stonks::app::dt::stec::endpoints