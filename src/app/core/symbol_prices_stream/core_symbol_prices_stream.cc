#include "core_symbol_prices_stream.h"

#include <absl/strings/ascii.h>
#include <fmt/core.h>

#include <string>
#include <utility>

#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "network_ws_types.h"

namespace stonks::core {
namespace {
auto BookTickerEndpointFor [[nodiscard]] (const Symbol &symbol) {
  return network::WsEndpoint{
      fmt::format("wss://stream.binance.com:9443/ws/{}@bookTicker",
                  absl::AsciiStrToLower(*symbol))};
}
}  // namespace

SymbolPricesStream::SymbolPricesStream(Symbol symbol,
                                       cpp::NnUp<ISymbolsDb> symbols_db,
                                       cpp::NnUp<network::IWsClient> ws_client)
    : web_socket_{
          BookTickerEndpointFor(symbol), std::move(ws_client),
          sps::BookTickHandler{std::move(symbol), std::move(symbols_db)}} {}

}  // namespace stonks::core