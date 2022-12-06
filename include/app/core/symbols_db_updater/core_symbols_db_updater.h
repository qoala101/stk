#ifndef STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_
#define STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_

#include <absl/time/time.h>

#include "core_sdbu_old_prices_deleter.h"
#include "core_sdbu_symbols_info_updater.h"
#include "cpp_timer.h"

namespace stonks::core {
/**
 * @brief Periodically updates Symbols DB.
 */
class SymbolsDbUpdater {
 public:
  SymbolsDbUpdater(sdbu::SymbolsInfoUpdater symbols_info_updater,
                   absl::Duration update_symbols_info_interval,
                   sdbu::OldPricesDeleter old_prices_deleter,
                   absl::Duration delete_old_prices_interval);

 private:
  cpp::Timer update_symbols_info_timer_;
  cpp::Timer delete_old_prices_timer_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SYMBOLS_DB_UPDATER_H_
