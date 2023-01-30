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

auto SymbolPricesFilter::InsertSymbolPriceRecord(SymbolPriceRecord record)
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

  co_await Proxy::InsertSymbolPriceRecord(std::move(record));
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
  auto records =
      co_await SelectSymbolPriceRecords(symbol, order, {}, {}, limit);

  if (records.empty()) {
    co_return std::nullopt;
  }

  co_return std::move(records.front());
}
}  // namespace stonks::core