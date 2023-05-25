/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_CORE_BUY_LOW_SELL_HIGH_H_
#define VH_STK_CORE_BUY_LOW_SELL_HIGH_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_blsh_types.h"
#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_meta_thread_safe.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"

namespace vh::stk::core {
class BuyLowSellHigh {
 public:
  explicit BuyLowSellHigh(
      cpp::meta::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db);

  auto CalculateNextOperations
      [[nodiscard]] (Symbol symbol, double profit,
                     const blsh::Operation &last_operation,
                     const cpp::Opt<absl::Time> &start_time,
                     const cpp::Opt<absl::Time> &end_time) const
      -> cppcoro::task<std::vector<blsh::Operation>>;

 private:
  cpp::NnUp<ISymbolsDb> symbols_db_;
  double commission_{};
};
}  // namespace vh::stk::core

#endif  // VH_STK_CORE_BUY_LOW_SELL_HIGH_H_
