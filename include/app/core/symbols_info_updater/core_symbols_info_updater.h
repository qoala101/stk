#ifndef STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SYMBOLS_INFO_UPDATER_H_
#define STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SYMBOLS_INFO_UPDATER_H_

#include <absl/time/time.h>

#include "binance_api.h"
#include "core_i_symbols_db.h"
#include "cpp_not_null.h"
#include "cpp_timer.h"

namespace stonks::core {
/**
 * @brief Polls symbols info from Binance at the specified interval
 * and it in the Symbols DB.
 */
class SymbolsInfoUpdater {
 public:
  explicit SymbolsInfoUpdater(cpp::NnUp<ISymbolsDb> symbols_db,
                              binance::BinanceApi binance_api,
                              absl::Duration update_interval);

 private:
  cpp::Timer timer_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SYMBOLS_INFO_UPDATER_H_
