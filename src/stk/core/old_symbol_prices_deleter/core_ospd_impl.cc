#include "core_ospd_impl.h"

#include <absl/time/clock.h>

#include <coroutine>
#include <utility>

namespace vh::stk::core::ospd {
Impl::Impl(cpp::NnUp<ISymbolsDb> symbols_db,
           absl::Duration keep_prices_for_duration)
    : symbols_db_{std::move(symbols_db)},
      keep_prices_for_duration_{keep_prices_for_duration} {}

auto Impl::DeleteOldPrices() const -> cppcoro::task<> {
  const auto end_time = absl::Now() - keep_prices_for_duration_;
  co_await symbols_db_->DeleteSymbolPriceRecords({}, end_time);
}
}  // namespace vh::stk::core::ospd