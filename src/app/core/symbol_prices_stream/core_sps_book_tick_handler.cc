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
                           .buy_price = {std::stod(book_tick.best_ask_price)},
                           .sell_price = {std::stod(book_tick.best_bid_price)},
                           .time = absl::Now()};
}

auto BookTickHandler::RecordAsPrice(binance::BookTick book_tick)
    -> cppcoro::task<> {
  auto record = SymbolPriceRecordFrom(book_tick);

  if (const auto price_not_changed =
          last_record_.has_value() &&
          (*last_record_->buy_price == record.buy_price) &&
          (*last_record_->sell_price == record.sell_price)) {
    co_return;
  }

  try {
    co_await symbols_db_->InsertSymbolPriceRecord(record);
    last_record_ = std::move(record);
  } catch (...) {
    co_return;
  }
}
}  // namespace stonks::core::sps