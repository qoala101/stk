#include "app_sps_app.h"

#include <absl/time/clock.h>

#include <utility>

#include "app_sps_sdb_app_client.h"
#include "app_sps_types.h"
#include "core_types.h"
#include "network_auto_parsable.h"
#include "network_ws_client_builder.h"

namespace stonks::app::sps {
namespace {
[[nodiscard]] auto SymbolPriceRecordFrom(core::Symbol symbol,
                                         const BinanceBookTick &book_tick) {
  return core::SymbolPriceRecord{
      .symbol = std::move(symbol),
      .price = {(std::stod(book_tick.best_bid_price) +
                 std::stod(book_tick.best_ask_price)) /
                2},
      .time = absl::Now()};
}
}  // namespace

auto App::BinanceSymbolBookTickerStream(core::Symbol symbol,
                                        SdbAppClient sdb_app_client) {
  return [symbol = std::move(symbol), sdb_client = std::move(sdb_app_client)](
             network::AutoParsable message) mutable {
    auto record = SymbolPriceRecordFrom(symbol, message);
    sdb_client.InsertSymbolPriceRecord(std::move(record));
  };
}

App::App(core::Symbol symbol, network::WsClientBuilder ws_client_builder,
         SdbAppClient sdb_app_client)
    : ws_connection_{ws_client_builder
                         .Handling(BinanceSymbolBookTickerStream(
                             std::move(symbol), std::move(sdb_app_client)))
                         .Connect()} {}
}  // namespace stonks::app::sps