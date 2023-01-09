#ifndef STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_
#define STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_

#include <absl/time/time.h>

#include "core_i_symbols_db_updater.h"
#include "core_sdbu_old_prices_deleter.h"
#include "core_sdbu_symbols_info_updater.h"
#include "cpp_timer.h"

namespace stonks::core {
/**
 * @copydoc ISymbolsDbUpdater
 */
class SymbolsDbUpdater : public ISymbolsDbUpdater {
 public:
  /**
   * @param reattempt_interval Time in which to reattempt updates if they fail.
   * Fail could occur if DB or Binance are not available.
   */
  SymbolsDbUpdater(absl::Duration update_symbols_info_interval,
                   sdbu::SymbolsInfoUpdater symbols_info_updater,
                   absl::Duration delete_old_prices_interval,
                   sdbu::OldPricesDeleter old_prices_deleter,
                   absl::Duration reattempt_interval);

  /**
   * @copydoc ISymbolsDbUpdater::GetUpdateSymbolsInfoInterval
   */
  auto GetUpdateSymbolsInfoInterval [[nodiscard]] () const
      -> cppcoro::task<absl::Duration> override;

 private:
  absl::Duration update_symbols_info_interval_{};
  cpp::Timer update_symbols_info_timer_;
  cpp::Timer delete_old_prices_timer_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_
