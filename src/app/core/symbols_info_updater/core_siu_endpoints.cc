#include "core_siu_endpoints.h"

#include <vector>

#include "core_siu_types.h"  // IWYU pragma: keep
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_typed_endpoint.h"

namespace stonks::core::siu::endpoints {
auto BinanceExchangeInfo() -> network::TypedEndpoint {
  return {
      .endpoint = {.method = network::Method::kGet, .uri = {"/exchangeInfo"}},
      .expected_types = {
          .result = network::ExpectedType<struct BinanceExchangeInfo>()}};
};
}  // namespace stonks::core::siu::endpoints