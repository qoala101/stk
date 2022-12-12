#include "core_buy_low_sell_high.h"

#include <coroutine>
#include <utility>

namespace stonks::core {
BuyLowSellHigh::BuyLowSellHigh(cpp::NnUp<ISymbolsDb> symbols_db)
    : symbols_db_{std::move(symbols_db)} {}

auto BuyLowSellHigh::CalculateNextOperations(
    const Symbol &symbol, double profit, const blsh::Operation &last_operation,
    const absl::Time *start_time, const absl::Time *end_time) const
    -> cppcoro::task<std::vector<blsh::Operation>> {
  co_return std::vector<blsh::Operation>{};
}
}  // namespace stonks::core