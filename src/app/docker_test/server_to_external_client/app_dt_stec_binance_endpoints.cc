#include "app_dt_stec_binance_endpoints.h"

#include <string>

#include "app_dt_stec_binance_types.h"
#include "app_dt_types.h"
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_typed_endpoint.h"

namespace stonks::app::dt::stec::binance::endpoints {
auto GetCurrentAveragePrice() -> network::TypedEndpoint {
  return {.endpoint = {.method = network::Method::kGet, .uri = {"/avgPrice"}},
          .expected_types = {
              .params = {{"symbol", network::ExpectedType<Symbol>()}},
              .result = network::ExpectedType<AvgPrice>()}};
}
}  // namespace stonks::app::dt::stec::binance::endpoints