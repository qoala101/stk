#include "core_symbols_db_updater.h"

#include <cppcoro/sync_wait.hpp>
#include <function2/function2.hpp>
#include <type_traits>
#include <utility>

#include "core_sdbu_old_prices_deleter.h"
#include "core_sdbu_symbols_info_updater.h"

namespace stonks::core {
SymbolsDbUpdater::SymbolsDbUpdater(
    sdbu::SymbolsInfoUpdater symbols_info_updater,
    absl::Duration update_symbols_info_interval,
    sdbu::OldPricesDeleter old_prices_deleter,
    absl::Duration delete_old_prices_interval)
    : update_symbols_info_timer_{[updater = std::move(symbols_info_updater)]() {
                                   cppcoro::sync_wait(
                                       updater.GetAndUpdateSymbolsInfo());
                                 },
                                 update_symbols_info_interval},
      delete_old_prices_timer_{[deleter = std::move(old_prices_deleter)]() {
                                 cppcoro::sync_wait(deleter.DeleteOldPrices());
                               },
                               delete_old_prices_interval} {}
}  // namespace stonks::core