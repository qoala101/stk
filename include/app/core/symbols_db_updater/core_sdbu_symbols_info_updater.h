#ifndef STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SDBU_SYMBOLS_INFO_UPDATER_H_
#define STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SDBU_SYMBOLS_INFO_UPDATER_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>

#include "binance_api.h"
#include "core_i_symbols_db.h"
#include "cpp_not_null.h"

namespace stonks::core::sdbu {
/**
 * @brief Updates assets and symbols info in DB.
 */
class SymbolsInfoUpdater {
 public:
  SymbolsInfoUpdater(cpp::NnSp<ISymbolsDb> symbols_db,
                     binance::BinanceApi binance_api,
                     absl::Duration update_symbols_info_interval);

  /**
   * @brief Receives symbols info from Binance and update Symbols DB if needed.
   */
  auto GetAndUpdateSymbolsInfo [[nodiscard]] () -> cppcoro::task<>;

 private:
  cpp::NnSp<ISymbolsDb> symbols_db_;
  binance::BinanceApi binance_api_;
  absl::Duration update_symbols_info_interval_{};
  absl::Time last_update_time_{};
};
}  // namespace stonks::core::sdbu

#endif  // STONKS_APP_CORE_SYMBOLS_DB_UPDATER_CORE_SDBU_SYMBOLS_INFO_UPDATER_H_
