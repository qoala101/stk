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
#include "cpp_message_exception.h"
#include "cpp_typed_struct.h"

namespace stonks::core::sps {
namespace {
auto GetBaseAssetPriceStep(const Symbol &symbol, const ISymbolsDb &symbols_db)
    -> cppcoro::task<double> {
  auto symbol_info = co_await symbols_db.SelectSymbolInfo(symbol);

  if (!symbol_info.has_value()) {
    throw cpp::MessageException{
        fmt::format("Couldn't get symbol info for {}", *symbol)};
  }

  co_return symbol_info->base_asset.price_step;
}
}  // namespace

BookTickHandler::BookTickHandler(Symbol symbol,
                                 cpp::NnUp<ISymbolsDb> symbols_db)
    : symbol_{std::move(symbol)},
      symbols_db_{std::move(symbols_db)},
      base_asset_price_step_{
          [symbol = symbol_,
           symbols_db = symbols_db_]() -> cppcoro::task<double> {
            return GetBaseAssetPriceStep(symbol, *symbols_db);
          },
          absl::Seconds(1)},
      last_record_{cppcoro::sync_wait(GetLastPriceRecord())} {}

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
  auto record = co_await SymbolPriceRecordFrom(book_tick);

  if (const auto price_not_changed =
          (*last_record_.buy_price == record.buy_price) &&
          (*last_record_.sell_price == record.sell_price)) {
    co_return;
  }

  try {
    co_await symbols_db_->InsertSymbolPriceRecord(record);
    last_record_ = std::move(record);
  } catch (...) {
  }
}

auto BookTickHandler::GetLastPriceRecord() -> cppcoro::task<SymbolPriceRecord> {
  const auto order = TimeOrder::kNewFirst;
  const auto limit = 1;

  auto records = std::vector<SymbolPriceRecord>{};

  try {
    records = co_await symbols_db_->SelectSymbolPriceRecords(
        symbol_, &order, nullptr, nullptr, &limit);
  } catch (...) {
    co_return SymbolPriceRecord{};
  }

  if (records.empty()) {
    co_return SymbolPriceRecord{};
  }

  co_return std::move(records.front());
}
}  // namespace stonks::core::sps