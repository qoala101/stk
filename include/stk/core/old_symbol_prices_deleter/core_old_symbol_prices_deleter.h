#ifndef STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_
#define STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>

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

#endif  // STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_
