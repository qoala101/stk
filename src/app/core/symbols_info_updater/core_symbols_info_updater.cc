#include "core_symbols_info_updater.h"

#include <cppcoro/sync_wait.hpp>
#include <memory>
#include <not_null.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>
#include <vector>

#include "binance_api.h"
#include "core_types.h"
#include "cpp_typed_struct.h"

namespace stonks::core {
namespace {
auto ToSymbolInfo
    [[nodiscard]] (const binance::SymbolExchangeInfo &binance_info) {
  return SymbolInfo{
      .symbol = {binance_info.symbol},
      .base_asset = {.asset = {binance_info.base_asset},
                     .min_amount = std::stod(binance_info.min_notional),
                     .price_step = std::stod(binance_info.step_size)},
      .quote_asset = {.asset = {binance_info.quote_asset},
                      .min_amount = std::stod(binance_info.min_quantity),
                      .price_step = std::stod(binance_info.tick_size)}};
}
}  // namespace

SymbolsInfoUpdater::SymbolsInfoUpdater(absl::Duration interval,
                                       cpp::NnUp<ISymbolsDb> symbols_db,
                                       binance::BinanceApi binance_api)
    : binance_api_{std::move(binance_api)} {
  auto exchange_info = cppcoro::sync_wait(binance_api_.exchangeInfo());
  auto symbols_info = *exchange_info | ranges::views::move |
                      ranges::views::transform(&ToSymbolInfo) |
                      ranges::to_vector;
  cppcoro::sync_wait(symbols_db->UpdateSymbolsInfo(std::move(symbols_info)));
}
}  // namespace stonks::core