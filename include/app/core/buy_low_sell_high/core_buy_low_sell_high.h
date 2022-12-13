#ifndef STONKS_APP_CORE_BUY_LOW_SELL_HIGH_CORE_BUY_LOW_SELL_HIGH_H_
#define STONKS_APP_CORE_BUY_LOW_SELL_HIGH_CORE_BUY_LOW_SELL_HIGH_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_blsh_types.h"
#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_not_null.h"

namespace stonks::core {
class BuyLowSellHigh {
 public:
  explicit BuyLowSellHigh(cpp::NnUp<ISymbolsDb> symbols_db);

  auto CalculateNextOperations
      [[nodiscard]] (const Symbol &symbol, double profit,
                     const blsh::Operation &last_operation,
                     const absl::Time *start_time,
                     const absl::Time *end_time) const
      -> cppcoro::task<std::vector<blsh::Operation>>;

 private:
  cpp::NnUp<ISymbolsDb> symbols_db_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_BUY_LOW_SELL_HIGH_CORE_BUY_LOW_SELL_HIGH_H_