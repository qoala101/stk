#include "core_sps_price_recorder.h"

#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <fmt/core.h>

#include <coroutine>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <not_null.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core_types.h"

namespace stonks::core::sps {
PriceRecorder::PriceRecorder(Symbol symbol, cpp::NnSp<ISymbolsDb> symbols_db)
    : symbol_{std::move(symbol)}, symbols_db_{std::move(symbols_db)} {}

auto PriceRecorder::SymbolPriceRecordFrom(
    const binance::BookTick &book_tick) const {
  return SymbolPriceRecord{.symbol = symbol_,
                           .buy_price = {std::stod(book_tick.best_ask_price)},
                           .sell_price = {std::stod(book_tick.best_bid_price)},
                           .time = absl::Now()};
}

auto PriceRecorder::RecordAsPrice(binance::BookTick book_tick)
    -> cppcoro::task<> {
  auto price_record = SymbolPriceRecordFrom(book_tick);

  try {
    co_await symbols_db_->InsertSymbolPriceRecord(std::move(price_record));
  } catch (...) {
  }
}
}  // namespace stonks::core::sps