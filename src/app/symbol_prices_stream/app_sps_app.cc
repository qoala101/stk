#include "app_sps_app.h"

#include <absl/time/clock.h>

#include <utility>

#include "app_sps_endpoints.h"
#include "app_sps_sdb_client.h"
#include "app_sps_types.h"
#include "core_types.h"
#include "network_auto_parsable.h"
#include "network_json_basic_conversions.h"
#include "network_ws_client_builder.h"

namespace stonks::app::sps {
namespace {
[[nodiscard]] auto SymbolPriceRecordFrom(core::Symbol symbol,
                                         const BinanceSymbolBookTick &book_tick)
    -> core::SymbolPriceRecord {
  return {.symbol = std::move(symbol),
          .price = {(book_tick.best_bid_price + book_tick.best_ask_price) / 2},
          .time = absl::Now()};
}
}  // namespace

App::App(core::Symbol symbol, SdbClient sdb_client,
         cpp::NnUp<network::IWsClient> ws_client)
    : ws_connection_{network::WsClientBuilder{std::move(ws_client)}
                         .On(endpoints::BinanceSymbolBookTickerStream(symbol))
                         .Handling(BinanceSymbolBookTickerStream(
                             std::move(symbol), std::move(sdb_client)))
                         .Connect()} {}

auto App::BinanceSymbolBookTickerStream(core::Symbol symbol,
                                        SdbClient sdb_client)
    -> network::aprh::HandlerWithWsMessage {
  return [symbol = std::move(symbol), sdb_client = std::move(sdb_client)](
             network::AutoParsable message) mutable {
    auto record = SymbolPriceRecordFrom(symbol, message);
    sdb_client.InsertSymbolPriceRecord(std::move(record));
  };
}
}  // namespace stonks::app::sps