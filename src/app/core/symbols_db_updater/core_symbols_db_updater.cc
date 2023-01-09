#include "core_symbols_db_updater.h"

#include <absl/time/time.h>

#include <cppcoro/sync_wait.hpp>
#include <type_traits>
#include <utility>

#include "core_sdbu_old_prices_deleter.h"
#include "core_sdbu_symbols_info_updater.h"
#include "cpp_timer_builder.h"

namespace stonks::core {
SymbolsDbUpdater::SymbolsDbUpdater(
    absl::Duration update_symbols_info_interval,
    sdbu::SymbolsInfoUpdater symbols_info_updater,
    absl::Duration delete_old_prices_interval,
    sdbu::OldPricesDeleter old_prices_deleter,
    absl::Duration reattempt_interval)
    : update_symbols_info_interval_{update_symbols_info_interval},
      update_symbols_info_timer_{
          cpp::Execute([updater = std::move(symbols_info_updater)]() {
            cppcoro::sync_wait(updater.GetAndUpdateSymbolsInfo());
          })
              .Every(update_symbols_info_interval_)
              .IfThrowsReattemptEvery(reattempt_interval)
              .Start()},
      delete_old_prices_timer_{
          cpp::Execute([deleter = std::move(old_prices_deleter)]() {
            cppcoro::sync_wait(deleter.DeleteOldPrices());
          })
              .Every(delete_old_prices_interval)
              .IfThrowsReattemptEvery(reattempt_interval)
              .Start()} {}

auto SymbolsDbUpdater::GetUpdateSymbolsInfoInterval [[nodiscard]] () const
    -> cppcoro::task<absl::Duration> {
  co_return update_symbols_info_interval_;
}
}  // namespace stonks::core