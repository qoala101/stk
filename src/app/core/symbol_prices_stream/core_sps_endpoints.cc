#include "core_sps_endpoints.h"

#include <absl/strings/ascii.h>
#include <fmt/core.h>

#include "core_sps_types.h"
#include "core_types.h"
#include "cpp_typed_struct.h"
#include "network_typed_endpoint.h"

namespace stonks::core::sps::endpoints {
auto BinanceSymbolBookTickerStream(Symbol symbol) -> network::TypedWsEndpoint {
  absl::AsciiStrToLower(&*symbol);
  return {.endpoint = {fmt::format(
              "wss://stream.binance.com:9443/ws/{}@bookTicker", *symbol)},
          .expected_types = {.received_message =
                                 network::ExpectedType<BinanceBookTick>()}};
};
}  // namespace stonks::core::sps::endpoints