#include "core_symbols_db_updater.h"

#include <absl/time/time.h>

#include <coroutine>
#include <cppcoro/sync_wait.hpp>
#include <utility>

#include "core_sdbu_old_prices_deleter.h"
#include "core_sdbu_symbols_info_updater.h"
#include "cpp_share.h"
#include "cpp_timer_builder.h"

namespace stonks::core {
SymbolsDbUpdater::SymbolsDbUpdater(ConstructorArgs args)
    : impl_{[&args]() {
        auto shared_symbols_db = cpp::Share(std::move(*args.symbols_db));
        return Impl{
            .update_symbols_info_interval{args.update_symbols_info_interval},
            .update_symbols_info_timer{
                cpp::Execute([updater =
                                  sdbu::SymbolsInfoUpdater{
                                      shared_symbols_db,
                                      std::move(args.binance_api)}]() {
                  cppcoro::sync_wait(updater.GetAndUpdateSymbolsInfo());
                })
                    .Every(args.update_symbols_info_interval)
                    .IfThrowsReattemptEvery(args.reattempt_interval)
                    .Start()},
            .delete_old_prices_timer{
                cpp::Execute([deleter =
                                  sdbu::OldPricesDeleter{
                                      std::move(shared_symbols_db),
                                      args.keep_prices_for_duration}]() {
                  cppcoro::sync_wait(deleter.DeleteOldPrices());
                })
                    .Every(args.delete_old_prices_interval)
                    .IfThrowsReattemptEvery(args.reattempt_interval)
                    .Start()}};
      }()} {}

auto SymbolsDbUpdater::GetUpdateSymbolsInfoInterval [[nodiscard]] () const
    -> cppcoro::task<absl::Duration> {
  co_return impl_.update_symbols_info_interval;
}
}  // namespace stonks::core