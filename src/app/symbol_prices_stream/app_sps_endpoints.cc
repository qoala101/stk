#include "app_sps_endpoints.h"

#include <absl/strings/ascii.h>
#include <fmt/core.h>

#include "app_sps_types.h"
#include "core_types.h"
#include "cpp_optional.h"
#include "network_typed_endpoint.h"

namespace stonks::app::sps::endpoints {
auto BinanceSymbolBookTickerStream(core::Symbol symbol)
    -> network::TypedWsEndpoint {
  absl::AsciiStrToLower(&symbol.value);
  return {.endpoint = {fmt::format(
              "wss://stream.binance.com:9443/ws/{}@bookTicker", symbol.value)},
          .expected_types = {.received_message =
                                 network::ExpectedType<BinanceBookTick>()}};
};
}  // namespace stonks::app::sps::endpoints