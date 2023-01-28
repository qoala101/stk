#include "core_symbol_prices_filter.h"

#include <fmt/format.h>

#include <coroutine>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

#include "core_common.h"
#include "cpp_message_exception.h"

namespace stonks::core {
namespace {
auto RoundPricesToStep(SymbolPriceRecord &record,
                       double base_asset_price_step) {
  record.buy_price = {
      Ceil({.value = record.buy_price, .precision = base_asset_price_step})};
  record.sell_price = {
      Floor({.value = record.sell_price, .precision = base_asset_price_step})};
}
}  // namespace

SymbolPricesFilter::SymbolPricesFilter(
    cpp::meta::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db)
    : symbols_db_{std::move(symbols_db)} {}

auto SymbolPricesFilter::SelectAssets() const
    -> cppcoro::task<std::vector<core::Asset>> {
  co_return co_await symbols_db_->SelectAssets();
}

auto SymbolPricesFilter::UpdateAssets(std::vector<core::Asset> assets)
    -> cppcoro::task<> {
  co_await symbols_db_->UpdateAssets(std::move(assets));
}

auto SymbolPricesFilter::SelectSymbolsWithPriceRecords() const
    -> cppcoro::task<std::vector<core::Symbol>> {
  co_await symbols_db_->SelectSymbolsWithPriceRecords();
}

auto SymbolPricesFilter::SelectSymbolInfo(core::Symbol symbol) const
    -> cppcoro::task<cpp::Opt<core::SymbolInfo>> {
  co_return co_await symbols_db_->SelectSymbolInfo(std::move(symbol));
}

auto SymbolPricesFilter::SelectSymbolsInfo() const
    -> cppcoro::task<std::vector<core::SymbolInfo>> {
  co_return co_await symbols_db_->SelectSymbolsInfo();
}

auto SymbolPricesFilter::UpdateSymbolsInfo(std::vector<core::SymbolInfo> infos)
    -> cppcoro::task<> {
  co_await symbols_db_->UpdateSymbolsInfo(std::move(infos));
}

auto SymbolPricesFilter::SelectSymbolPriceRecords(const core::Symbol &symbol,
                                                  const core::TimeOrder *order,
                                                  const absl::Time *start_time,
                                                  const absl::Time *end_time,
                                                  const int *limit) const
    -> cppcoro::task<std::vector<core::SymbolPriceRecord>> {
  co_return co_await symbols_db_->SelectSymbolPriceRecords(
      symbol, order, start_time, end_time, limit);
}

auto SymbolPricesFilter::InsertSymbolPriceRecord(core::SymbolPriceRecord record)
    -> cppcoro::task<> {
  const auto last_price_record = co_await GetLastPriceRecord(record.symbol);
  const auto base_asset_price_step =
      co_await GetBaseAssetPriceStep(record.symbol);

  RoundPricesToStep(record, base_asset_price_step);

  if (const auto price_not_changed =
          last_price_record.has_value() &&
          (*last_price_record->buy_price == record.buy_price) &&
          (*last_price_record->sell_price == record.sell_price)) {
    co_return;
  }

  co_await symbols_db_->InsertSymbolPriceRecord(std::move(record));
}

auto SymbolPricesFilter::DeleteSymbolPriceRecords(const absl::Time *start_time,
                                                  const absl::Time *end_time)
    -> cppcoro::task<> {
  co_await symbols_db_->DeleteSymbolPriceRecords(start_time, end_time);
}

auto SymbolPricesFilter::GetBaseAssetPriceStep(const Symbol &symbol) const
    -> cppcoro::task<double> {
  auto symbol_info = co_await SelectSymbolInfo(symbol);

  if (!symbol_info.has_value()) {
    throw cpp::MessageException{
        fmt::format("Couldn't get symbol info for {}", *symbol)};
  }

  co_return symbol_info->base_asset.price_step;
}

auto SymbolPricesFilter::GetLastPriceRecord(const Symbol &symbol) const
    -> cppcoro::task<cpp::Opt<SymbolPriceRecord>> {
  const auto order = TimeOrder::kNewFirst;
  const auto limit = 1;

  auto records = co_await SelectSymbolPriceRecords(symbol, &order, nullptr,
                                                   nullptr, &limit);

  if (records.empty()) {
    co_return std::nullopt;
  }

  co_return std::move(records.front());
}
}  // namespace stonks::core