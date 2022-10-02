#include "app_sps_app.h"

#include <absl/time/clock.h>
#include <fmt/core.h>

#include <utility>

#include "app_sps_endpoints.h"
#include "app_sps_sdb_client.h"
#include "app_sps_types.h"
#include "core_types.h"
#include "network_auto_parsable.h"
#include "network_json_basic_conversions.h"
#include "network_ws_client_builder.h"

namespace stonks::app::sps {
App::App(cpp::NnUp<network::IWsClient> ws_client, core::Symbol symbol,
         SdbClient sdb_client)
    : ws_connection_{network::WsClientBuilder{std::move(ws_client)}
                         .On(endpoints::BinanceSymbolBookTickerStream(symbol))
                         .Handling([sdb_client = std::move(sdb_client), symbol](
                                       network::AutoParsable message) mutable {
                           const auto book_tick =
                               static_cast<BinanceSymbolBookTick>(message);

                           sdb_client.InsertSymbolPriceRecord(
                               {.symbol = symbol,
                                .price = {(book_tick.best_bid_price +
                                           book_tick.best_ask_price) /
                                          2},
                                .time = absl::Now()});
                         })
                         .Connect()} {}
}  // namespace stonks::app::sps