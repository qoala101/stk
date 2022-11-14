#ifndef STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SYMBOLS_INFO_UPDATER_H_
#define STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SYMBOLS_INFO_UPDATER_H_

#include <absl/time/time.h>

#include "core_i_symbols_db.h"
#include "core_siu_binance_client.h"
#include "cpp_not_null.h"

namespace stonks::core {
/**
 * @brief Polls symbols info from Binance at the specified interval
 * and updates them in the Symbols DB.
 */
class SymbolsInfoUpdater {
 public:
  SymbolsInfoUpdater(absl::Duration interval, siu::BinanceClient binance_client,
                     cpp::NnUp<ISymbolsDb> symbols_db);
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SYMBOLS_INFO_UPDATER_H_
