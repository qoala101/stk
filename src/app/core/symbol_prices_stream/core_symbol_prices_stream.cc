#include "core_symbol_prices_stream.h"

#include <absl/time/clock.h>

#include <memory>
#include <string>
#include <utility>

#include "core_sps_types.h"
#include "core_types.h"
#include "network_ws_client_builder.h"
#include "not_null.hpp"

namespace stonks::app::sps {
namespace {
auto SymbolPriceRecordFrom
    [[nodiscard]] (core::Symbol symbol, const BinanceBookTick &book_tick) {
  return core::SymbolPriceRecord{
      .symbol = std::move(symbol),
      .price = {(std::stod(book_tick.best_bid_price) +
                 std::stod(book_tick.best_ask_price)) /
                2},
      .time = absl::Now()};
}
}  // namespace

auto App::BinanceSymbolBookTickerStream(core::Symbol symbol,
                                        cpp::NnUp<sdb::IApp> sdb_app) {
  return [symbol = std::move(symbol),
          sdb_app = std::move(sdb_app)](auto message) mutable {
    auto record = SymbolPriceRecordFrom(symbol, message);
    sdb_app->InsertSymbolPriceRecord(std::move(record));
  };
}

App::App(core::Symbol symbol, network::WsClientBuilder ws_client_builder,
         cpp::NnUp<sdb::IApp> sdb_app)
    : ws_connection_{ws_client_builder
                         .Handling(BinanceSymbolBookTickerStream(
                             std::move(symbol), std::move(sdb_app)))
                         .Connect()} {}
}  // namespace stonks::app::sps