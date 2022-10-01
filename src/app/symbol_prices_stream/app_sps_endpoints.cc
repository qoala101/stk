#include "app_sps_endpoints.h"

#include <fmt/core.h>

#include <utility>

#include "app_sps_types.h"
#include "core_types.h"
#include "cpp_optional.h"
#include "network_json_basic_conversions.h"
#include "network_typed_endpoint.h"

namespace stonks::app::sps::endpoints {
auto BinanceSymbolBookTickerStream(core::Symbol symbol)
    -> network::TypedWsEndpoint {
  return {
      .endpoint = {fmt::format("wss://stream.binance.com:9443/ws/{}@bookTicker",
                               std::move(symbol.value))},
      .expected_types = {.received_message =
                             network::ExpectedType<BinanceSymbolBookTick>()}};
};
}  // namespace stonks::app::sps::endpoints