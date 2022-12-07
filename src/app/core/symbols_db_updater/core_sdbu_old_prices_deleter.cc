#include "core_sdbu_old_prices_deleter.h"

#include <absl/time/clock.h>

#include <coroutine>
#include <utility>

namespace stonks::core::sdbu {
OldPricesDeleter::OldPricesDeleter(cpp::NnSp<ISymbolsDb> symbols_db,
                                   absl::Duration keep_prices_for_duration)
    : symbols_db_{std::move(symbols_db)},
      keep_prices_for_duration_{keep_prices_for_duration} {}

auto OldPricesDeleter::DeleteOldPrices [[nodiscard]] () const
    -> cppcoro::task<> {
  const auto end_time = absl::Now() - keep_prices_for_duration_;
  co_await symbols_db_->DeleteSymbolPriceRecords(nullptr, &end_time);
}
}  // namespace stonks::core::sdbu