#include "core_symbols_info_updater.h"

#include <array>
#include <compare>
#include <coroutine>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <function2/function2.hpp>
#include <range/v3/action/action.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/utility/get.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>
#include <vector>

#include "binance_api.h"
#include "binance_types.h"
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

class SymbolsInfoUpdaterImpl {
 public:
  SymbolsInfoUpdaterImpl(cpp::NnUp<ISymbolsDb> symbols_db,
                         binance::BinanceApi binance_api)
      : symbols_db_{std::move(symbols_db)},
        binance_api_{std::move(binance_api)} {}

  auto GetAndUpdateSymbolsInfo [[nodiscard]] () const -> cppcoro::task<> {
    auto exchange_info = co_await binance_api_.exchangeInfo();
    auto symbols_info = *exchange_info | ranges::views::move |
                        ranges::views::transform(ToSymbolInfo) |
                        ranges::to_vector;
    auto assets = symbols_info |
                  ranges::views::transform([](const auto &symbol_info) {
                    return std::array{symbol_info.base_asset.asset,
                                      symbol_info.quote_asset.asset};
                  }) |
                  ranges::views::join | ranges::to_vector |
                  ranges::actions::sort | ranges::actions::unique;

    co_await symbols_db_->UpdateAssets(std::move(assets));
    co_await symbols_db_->UpdateSymbolsInfo(std::move(symbols_info));
  }

 private:
  cpp::NnUp<ISymbolsDb> symbols_db_;
  binance::BinanceApi binance_api_;
};
}  // namespace

SymbolsInfoUpdater::SymbolsInfoUpdater(cpp::NnUp<ISymbolsDb> symbols_db,
                                       binance::BinanceApi binance_api,
                                       absl::Duration update_interval)
    : timer_{[impl = SymbolsInfoUpdaterImpl{std::move(symbols_db),
                                            std::move(binance_api)}]() {
               cppcoro::sync_wait(impl.GetAndUpdateSymbolsInfo());
             },
             update_interval} {}
}  // namespace stonks::core