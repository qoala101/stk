#include "app_dt_stec_binance_endpoints.h"

#include <string>

#include "app_dt_stec_binance_types.h"
#include "app_dt_types.h"
#include "cpp_optional.h"
#include "network_enums.h"
#include "network_json_basic_conversions.h"
#include "network_typed_endpoint.h"

namespace stonks::app::dt::stec::binance::endpoints {
auto GetCurrentAveragePrice() -> const network::TypedEndpoint& {
  static const auto endpoint = network::TypedEndpoint{
      .endpoint = {.method = network::Method::kGet, .uri = {"/avgPrice"}},
      .expected_types = {
          .params = {{"symbol", network::ExpectedType<Symbol>()}},
          .result = network::ExpectedType<AvgPrice>()}};
  return endpoint;
}
}  // namespace stonks::app::dt::stec::binance::endpoints