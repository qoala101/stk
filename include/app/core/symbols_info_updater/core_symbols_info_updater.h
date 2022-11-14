#ifndef STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SYMBOLS_INFO_UPDATER_H_
#define STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SYMBOLS_INFO_UPDATER_H_

#include <absl/time/time.h>

#include "core_i_symbols_db.h"
#include "core_siu_binance_client.h"

namespace stonks::app::siu {
/**
 * @brief Polls symbols info from Binance at the specified interval
 * and updates them in the Symbols DB.
 */
class App {
 public:
  App(absl::Duration interval, BinanceClient binance_client,
      cpp::NnUp<sdb::IApp> sdb_app);
};
}  // namespace stonks::app::siu

#endif  // STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SYMBOLS_INFO_UPDATER_H_
