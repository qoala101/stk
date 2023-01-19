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

#include "core_common.h"
#include "core_types.h"
#include "cpp_message_exception.h"
#include "cpp_typed_struct.h"

namespace stonks::core::sps {
PriceRecorder::PriceRecorder(Symbol symbol, cpp::NnSp<ISymbolsDb> symbols_db)
    : symbol_{std::move(symbol)}, symbols_db_{std::move(symbols_db)} {}

auto PriceRecorder::SymbolPriceRecordFrom(const binance::BookTick &book_tick,
                                          double base_asset_price_step) {
  return SymbolPriceRecord{
      .symbol = symbol_,
      .buy_price = {Ceil({.value = std::stod(book_tick.best_ask_price),
                          .precision = base_asset_price_step})},
      .sell_price = {Floor({.value = std::stod(book_tick.best_bid_price),
                            .precision = base_asset_price_step})},
      .time = absl::Now()};
}

auto PriceRecorder::RecordAsPrice(binance::BookTick book_tick)
    -> cppcoro::task<> {
  auto base_asset_price_step = double{};

  try {
    base_asset_price_step = co_await GetBaseAssetPriceStep();
  } catch (...) {
    co_return;
  }

  auto last_price_record = cpp::Opt<SymbolPriceRecord>{};

  try {
    last_price_record = co_await GetLastPriceRecord();
  } catch (...) {
  }

  auto new_price_record =
      SymbolPriceRecordFrom(book_tick, base_asset_price_step);

  if (const auto price_not_changed =
          last_price_record.has_value() &&
          (*last_price_record->buy_price == new_price_record.buy_price) &&
          (*last_price_record->sell_price == new_price_record.sell_price)) {
    co_return;
  }

  try {
    co_await symbols_db_->InsertSymbolPriceRecord(std::move(new_price_record));
  } catch (...) {
  }
}

auto PriceRecorder::GetBaseAssetPriceStep() const -> cppcoro::task<double> {
  auto symbol_info = co_await symbols_db_->SelectSymbolInfo(symbol_);

  if (!symbol_info.has_value()) {
    throw cpp::MessageException{
        fmt::format("Couldn't get symbol info for {}", *symbol_)};
  }

  co_return symbol_info->base_asset.price_step;
}

auto PriceRecorder::GetLastPriceRecord() const
    -> cppcoro::task<cpp::Opt<SymbolPriceRecord>> {
  const auto order = TimeOrder::kNewFirst;
  const auto limit = 1;

  auto records = co_await symbols_db_->SelectSymbolPriceRecords(
      symbol_, &order, nullptr, nullptr, &limit);

  if (records.empty()) {
    co_return std::nullopt;
  }

  co_return std::move(records.front());
}
}  // namespace stonks::core::sps