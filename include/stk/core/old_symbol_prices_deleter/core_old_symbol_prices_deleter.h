/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_CORE_OLD_SYMBOL_PRICES_DELETER_H_
#define VH_STK_CORE_OLD_SYMBOL_PRICES_DELETER_H_

#include <absl/time/time.h>

#include "core_i_symbols_db.h"
#include "cpp_not_null.h"
#include "cpp_timer.h"

namespace vh::stk::core {
/**
 * @brief Periodically deletes old symbol prices from Symbols DB.
 */
class OldSymbolPricesDeleter {
 public:
  struct ConstructorArgs {
    cpp::NnUp<ISymbolsDb> symbols_db;

    /**
     * @brief Default interval at which old prices are deleted from DB.
     */
    absl::Duration delete_old_prices_interval{};

    /**
     * @brief Maximum lifetime of prices.
     */
    absl::Duration keep_prices_for_duration{};

    /**
     * @brief Time in which to reattempt updates if they
     * fail. Fail could occur if DB is not available.
     */
    absl::Duration reattempt_interval{};
  };

  explicit OldSymbolPricesDeleter(ConstructorArgs args);

 private:
  cpp::Timer delete_old_prices_timer_;
};
}  // namespace vh::stk::core

#endif  // VH_STK_CORE_OLD_SYMBOL_PRICES_DELETER_H_
