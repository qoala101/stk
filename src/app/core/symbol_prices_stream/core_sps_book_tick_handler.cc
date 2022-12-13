#include "core_sps_book_tick_handler.h"

#include <absl/time/clock.h>

#include <coroutine>
#include <not_null.hpp>
#include <string>
#include <utility>

namespace stonks::core::sps {
BookTickHandler::BookTickHandler(Symbol symbol,
                                 cpp::NnUp<ISymbolsDb> symbols_db)
    : symbol_{std::move(symbol)}, symbols_db_{std::move(symbols_db)} {}

auto BookTickHandler::SymbolPriceRecordFrom(
    const binance::BookTick &book_tick) const {
  return SymbolPriceRecord{.symbol = symbol_,
                           .price = {(std::stod(book_tick.best_bid_price) +
                                      std::stod(book_tick.best_ask_price)) /
                                     2},
                           .time = absl::Now()};
}

auto BookTickHandler::RecordAsPrice(binance::BookTick book_tick)
    -> cppcoro::task<> {
  auto record = SymbolPriceRecordFrom(book_tick);
  const auto new_price = record.price;

  if (const auto price_not_changed = last_recorded_price_.has_value() &&
                                     (*last_recorded_price_ == new_price)) {
    co_return;
  }

  try {
    co_await symbols_db_->InsertSymbolPriceRecord(std::move(record));
    last_recorded_price_ = new_price;
  } catch (...) {
    co_return;
  }
}
}  // namespace stonks::core::sps