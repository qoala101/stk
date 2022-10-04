#include "app_siu_app.h"

#include <utility>
#include <vector>

#include "app_siu_binance_client.h"
#include "app_siu_sdb_app_client.h"
#include "app_siu_types.h"
#include "core_types.h"

namespace stonks::app::siu {
namespace {
[[nodiscard]] auto SymbolInfoFrom(BinanceSymbolExchangeInfo binance_info) {
  return core::SymbolInfo{
      .symbol = {std::move(binance_info.symbol)},
      .base_asset = {.asset = {std::move(binance_info.base_asset)},
                     .min_amount = std::stod(binance_info.min_notional),
                     .price_step = std::stod(binance_info.step_size)},
      .quote_asset = {.asset = {std::move(binance_info.quote_asset)},
                      .min_amount = std::stod(binance_info.min_quantity),
                      .price_step = std::stod(binance_info.tick_size)}};
}
}  // namespace

App::App(absl::Duration interval, BinanceClient binance_client,
         SdbAppClient sdb_app_client) {
  auto exchange_info = binance_client.BinanceExchangeInfo();

  for (auto &symbol_info : exchange_info.value) {
    sdb_app_client.InsertOrUpdateSymbolInfo(SymbolInfoFrom(symbol_info));
  }
}
}  // namespace stonks::app::siu