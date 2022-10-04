#ifndef STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_APP_H_
#define STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_APP_H_

#include <absl/time/time.h>

#include "app_siu_binance_client.h"
#include "app_siu_sdb_app_client.h"

namespace stonks::app::siu {
/**
 * @brief Polls symbols info from Binance at the specified interval
 * and updates them in the Symbols DB.
 */
class App {
 public:
  App(absl::Duration interval, BinanceClient binance_client,
      SdbAppClient sdb_app_client);
};
}  // namespace stonks::app::siu

#endif  // STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_APP_H_
