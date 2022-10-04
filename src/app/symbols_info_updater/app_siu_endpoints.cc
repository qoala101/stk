#include "app_siu_endpoints.h"

#include <vector>

#include "app_siu_types.h"
#include "cpp_optional.h"
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_typed_endpoint.h"

namespace stonks::app::siu::endpoints {
auto BinanceExchangeInfo() -> const network::TypedEndpoint& {
  static const auto kEndpoint = network::TypedEndpoint{
      .endpoint = {.method = network::Method::kGet,
                   .uri = {"https://api.binance.com/api/v3/exchangeInfo"}},
      .expected_types = {
          .result =
              network::ExpectedType<std::vector<BinanceSymbolExchangeInfo>>()}};
  return kEndpoint;
};
}  // namespace stonks::app::siu::endpoints