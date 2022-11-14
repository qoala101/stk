#include "core_siu_endpoints.h"

#include <vector>

#include "core_siu_types.h"
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_typed_endpoint.h"

namespace stonks::app::siu::endpoints {
auto BinanceExchangeInfo() -> network::TypedEndpoint {
  return {.endpoint = {.method = network::Method::kGet,
                       .uri = {"https://api.binance.com/api/v3/exchangeInfo"}},
          .expected_types = {
              .result = network::ExpectedType<struct BinanceExchangeInfo>()}};
};
}  // namespace stonks::app::siu::endpoints