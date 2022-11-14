#include "core_symbols_info_updater.h"

#include <memory>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>
#include <vector>

#include "core_siu_binance_client.h"
#include "core_siu_types.h"
#include "core_types.h"
#include "cpp_typed_struct.h"
#include "not_null.hpp"

namespace stonks::core {
namespace {
auto ToSymbolInfo [[nodiscard]] (siu::BinanceSymbolExchangeInfo binance_info) {
  return SymbolInfo{
      .symbol = {std::move(binance_info.symbol)},
      .base_asset = {.asset = {std::move(binance_info.base_asset)},
                     .min_amount = std::stod(binance_info.min_notional),
                     .price_step = std::stod(binance_info.step_size)},
      .quote_asset = {.asset = {std::move(binance_info.quote_asset)},
                      .min_amount = std::stod(binance_info.min_quantity),
                      .price_step = std::stod(binance_info.tick_size)}};
}
}  // namespace

SymbolsInfoUpdater::SymbolsInfoUpdater(absl::Duration interval,
                                       siu::BinanceClient binance_client,
                                       cpp::NnUp<ISymbolsDb> symbols_db) {
  auto exchange_info = binance_client.BinanceExchangeInfo();
  auto symbols_info = *exchange_info | ranges::views::move |
                      ranges::views::transform(&ToSymbolInfo) |
                      ranges::to_vector;
  symbols_db->UpdateSymbolsInfo(std::move(symbols_info));
}
}  // namespace stonks::core