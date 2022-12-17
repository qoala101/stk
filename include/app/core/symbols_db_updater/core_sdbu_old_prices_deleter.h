#ifndef STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SDBU_OLD_PRICES_DELETER_H_
#define STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SDBU_OLD_PRICES_DELETER_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>

#include "core_i_symbols_db.h"
#include "cpp_not_null.h"

namespace stonks::core::sdbu {
/**
 * @brief Deletes old symbol prices from Symbols DB.
 */
class OldPricesDeleter {
 public:
  OldPricesDeleter(cpp::NnUp<ISymbolsDb> symbols_db,
                   absl::Duration keep_prices_for_duration);

  /**
   * @brief Deletes old prices.
   */
  auto DeleteOldPrices [[nodiscard]] () const -> cppcoro::task<>;

 private:
  cpp::NnUp<ISymbolsDb> symbols_db_;
  absl::Duration keep_prices_for_duration_{};
};
}  // namespace stonks::core::sdbu

#endif  // STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SDBU_OLD_PRICES_DELETER_H_
