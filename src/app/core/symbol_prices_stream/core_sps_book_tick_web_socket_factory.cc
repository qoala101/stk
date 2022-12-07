#include "core_sps_book_tick_web_socket_factory.h"

#include <absl/strings/ascii.h>
#include <fmt/core.h>

#include <string>
#include <utility>

#include "cpp_typed_struct.h"

namespace stonks::core::sps {
namespace {
auto BookTickerEndpointFor [[nodiscard]] (const Symbol &symbol) {
  return network::WsEndpoint{
      fmt::format("wss://stream.binance.com:9443/ws/{}@bookTicker",
                  absl::AsciiStrToLower(*symbol))};
}
}  // namespace

BookTickWebSocketFactory::BookTickWebSocketFactory(
    Symbol symbol, di::Factory<ISymbolsDb> symbols_db_factory,
    di::Factory<network::IWsClient> ws_client_factory)
    : symbol_{std::move(symbol)},
      symbols_db_factory_{std::move(symbols_db_factory)},
      ws_client_factory_{std::move(ws_client_factory)},
      endpoint_{BookTickerEndpointFor(symbol_)} {}

auto BookTickWebSocketFactory::Create() const
    -> networkx::WebSocket<&BookTickHandler::RecordAsPrice> {
  return {endpoint_, ws_client_factory_.Create(),
          sps::BookTickHandler{symbol_, symbols_db_factory_.Create()}};
}
}  // namespace stonks::core::sps