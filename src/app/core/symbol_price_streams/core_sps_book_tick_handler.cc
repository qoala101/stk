#include "core_sps_book_tick_handler.h"

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

#include "core_common.h"
#include "core_types.h"
#include "cpp_typed_struct.h"

namespace stonks::core::sps {
BookTickHandler::BookTickHandler(
    Symbol symbol, cpp::NnSp<ISymbolsDb> symbols_db,
    cpp::AutoUpdatable<double> base_asset_price_step,
    cpp::Opt<SymbolPriceRecord> last_price_record)
    : symbol_{std::move(symbol)},
      symbols_db_{std::move(symbols_db)},
      base_asset_price_step_{std::move(base_asset_price_step)},
      last_price_record_{std::move(last_price_record)} {}

auto BookTickHandler::SymbolPriceRecordFrom(const binance::BookTick &book_tick)
    -> cppcoro::task<SymbolPriceRecord> {
  const auto lock = base_asset_price_step_.LockUpdates();
  co_return SymbolPriceRecord{
      .symbol = symbol_,
      .buy_price = {Ceil({.value = std::stod(book_tick.best_ask_price),
                          .precision = co_await *base_asset_price_step_})},
      .sell_price = {Floor({.value = std::stod(book_tick.best_bid_price),
                            .precision = co_await *base_asset_price_step_})},
      .time = absl::Now()};
}

auto BookTickHandler::RecordAsPrice(binance::BookTick book_tick)
    -> cppcoro::task<> {
  auto new_price_record = co_await SymbolPriceRecordFrom(book_tick);

  if (const auto price_not_changed =
          last_price_record_.has_value() &&
          (*last_price_record_->buy_price == new_price_record.buy_price) &&
          (*last_price_record_->sell_price == new_price_record.sell_price)) {
    co_return;
  }

  try {
    co_await symbols_db_->InsertSymbolPriceRecord(new_price_record);
    last_price_record_ = std::move(new_price_record);
  } catch (...) {
  }
}
}  // namespace stonks::core::sps