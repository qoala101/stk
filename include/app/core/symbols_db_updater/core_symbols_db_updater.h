#ifndef STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_
#define STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>

#include "binance_api.h"
#include "core_i_symbols_db.h"
#include "cpp_meta_thread_safe.h"
#include "cpp_not_null.h"
#include "cpp_timer.h"

namespace stonks::core {
/**
 * @brief Periodically updates Symbols DB.
 */
class SymbolsDbUpdater {
 public:
  struct ConstructorArgs {
    cpp::meta::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db;
    binance::BinanceApi binance_api;

    /**
     * @brief Default interval at which symbols info is updated on DB.
     */
    absl::Duration update_symbols_info_interval{};

    /**
     * @brief Interval at which DB would be checked whether it is in updated
     * state. If not, updates would happen immediately instead of waiting
     * for specified intervals.
     */
    absl::Duration check_if_update_required_interval{};

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
     * fail. Fail could occur if DB or Binance are not available.
     */
    absl::Duration reattempt_interval{};
  };

  explicit SymbolsDbUpdater(ConstructorArgs args);

 private:
  struct Impl {
    cpp::Timer update_symbols_info_timer;
    cpp::Timer delete_old_prices_timer;
  } impl_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_
