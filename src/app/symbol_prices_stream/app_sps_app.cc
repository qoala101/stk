#include "app_sps_app.h"

#include <fmt/core.h>
#include <utility>

#include "app_sps_endpoints.h"
#include "app_sps_types.h"
#include "core_types.h"
#include "network_ws_client_builder.h"
#include "network_auto_parsable.h"
#include "network_json_basic_conversions.h"

namespace stonks::app::sps {
App::App(cpp::NnUp<network::IWsClient> ws_client, core::Symbol symbol)
    : ws_connection_{
          network::WsClientBuilder{std::move(ws_client)}
              .On(endpoints::BinanceSymbolBookTickerStream(std::move(symbol)))
              .Handling([](network::AutoParsable message) {
                const auto book_tick = static_cast<BinanceSymbolBookTick>(message);
                fmt::print("Price {} {}", book_tick.best_bid_price,
                           book_tick.best_ask_price);
              })
              .Connect()} {}
}  // namespace stonks::app::sps