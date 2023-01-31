#include "core_old_symbol_prices_deleter.h"

#include <cppcoro/sync_wait.hpp>
#include <utility>

#include "core_ospd_impl.h"
#include "cpp_timer_builder.h"

namespace vh::stk::core {
OldSymbolPricesDeleter::OldSymbolPricesDeleter(ConstructorArgs args)
    : delete_old_prices_timer_{
          cpp::Execute([impl = ospd::Impl{std::move(args.symbols_db),
                                          args.keep_prices_for_duration}]() {
            cppcoro::sync_wait(impl.DeleteOldPrices());
          })
              .Every(args.delete_old_prices_interval)
              .IfThrowsReattemptEvery(args.reattempt_interval)
              .Start()} {}
}  // namespace vh::stk::core