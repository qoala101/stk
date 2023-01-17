#ifndef STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_
#define STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>

#include "binance_api.h"
#include "cpp_meta_thread_safe.h"
#include "core_i_symbols_db.h"
#include "core_i_symbols_db_updater.h"
#include "cpp_not_null.h"
#include "cpp_timer.h"

namespace stonks::core {
/**
 * @copydoc ISymbolsDbUpdater
 */
class SymbolsDbUpdater : public ISymbolsDbUpdater {
 public:
  struct ConstructorArgs {
    cpp::meta::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db;
    binance::BinanceApi binance_api;
    absl::Duration update_symbols_info_interval{};
    absl::Duration delete_old_prices_interval{};
    absl::Duration keep_prices_for_duration{};

    /**
     * @brief Time in which to reattempt updates if they
     * fail. Fail could occur if DB or Binance are not available.
     */
    absl::Duration reattempt_interval{};
  };

  explicit SymbolsDbUpdater(ConstructorArgs args);

  /**
   * @copydoc ISymbolsDbUpdater::GetUpdateSymbolsInfoInterval
   */
  auto GetUpdateSymbolsInfoInterval [[nodiscard]] () const
      -> cppcoro::task<absl::Duration> override;

 private:
  struct Impl {
    cpp::Timer update_symbols_info_timer;
    cpp::Timer delete_old_prices_timer;
    absl::Duration update_symbols_info_interval{};
  } impl_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_
