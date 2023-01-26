#include "core_symbols_info_updater.h"

#include <absl/time/time.h>

#include <coroutine>
#include <cppcoro/sync_wait.hpp>
#include <utility>

#include "core_siu_impl.h"
#include "cpp_timer_builder.h"

namespace stonks::core {
SymbolsInfoUpdater::SymbolsInfoUpdater(ConstructorArgs args)
    : update_symbols_info_timer_{
          cpp::Execute(
              [impl = siu::Impl{std::move(args.symbols_db),
                                std::move(args.binance_api),
                                args.update_symbols_info_interval}]() mutable {
                cppcoro::sync_wait(impl.GetAndUpdateSymbolsInfo());
              })
              .Every(args.check_if_update_required_interval)
              .IfThrowsReattemptEvery(args.reattempt_interval)
              .Start()} {}
}  // namespace stonks::core