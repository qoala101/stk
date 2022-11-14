#include "core_symbols_info_updater.h"

#include <range/v3/to_container.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <utility>
#include <vector>

#include "core_siu_binance_client.h"
#include "core_siu_types.h"
#include "core_types.h"
#include "cpp_typed_struct.h"

namespace stonks::app::siu {
namespace {
auto ToSymbolInfo [[nodiscard]] (BinanceSymbolExchangeInfo binance_info) {
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
         cpp::NnUp<sdb::IApp> sdb_app) {
  auto exchange_info = binance_client.BinanceExchangeInfo();
  auto symbols_info = *exchange_info | ranges::views::move |
                      ranges::views::transform(&ToSymbolInfo) |
                      ranges::to_vector;
  sdb_app->UpdateSymbolsInfo(std::move(symbols_info));
}
}  // namespace stonks::app::siu