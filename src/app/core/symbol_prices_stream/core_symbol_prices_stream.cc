#include "core_symbol_prices_stream.h"

#include <absl/time/clock.h>

#include <memory>
#include <not_null.hpp>
#include <string>
#include <utility>

#include "core_sps_types.h"
#include "core_types.h"
#include "network_ws_client_builder.h"

namespace stonks::core {
namespace {
auto SymbolPriceRecordFrom
    [[nodiscard]] (Symbol symbol, const sps::BinanceBookTick &book_tick) {
  return SymbolPriceRecord{.symbol = std::move(symbol),
                           .price = {(std::stod(book_tick.best_bid_price) +
                                      std::stod(book_tick.best_ask_price)) /
                                     2},
                           .time = absl::Now()};
}
}  // namespace

auto SymbolPricesStream::BinanceSymbolBookTickerStream(
    Symbol symbol, cpp::NnUp<ISymbolsDb> symbols_db) {
  return [symbol = std::move(symbol),
          symbols_db = std::move(symbols_db)](auto message) mutable {
    auto record = SymbolPriceRecordFrom(symbol, message);
    symbols_db->InsertSymbolPriceRecord(std::move(record));
  };
}

SymbolPricesStream::SymbolPricesStream(
    Symbol symbol, network::WsClientBuilder ws_client_builder,
    cpp::NnUp<ISymbolsDb> symbols_db)
    : ws_connection_{ws_client_builder
                         .Handling(BinanceSymbolBookTickerStream(
                             std::move(symbol), std::move(symbols_db)))
                         .Connect()} {}
}  // namespace stonks::core