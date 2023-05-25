/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

// clang-format off
#include "core_sdb_value_conversions.h"   // IWYU pragma: keep
// clang-format on

#include "core_symbols_db.h"

#include <absl/time/time.h>

#include <compare>
#include <concepts>
#include <coroutine>
#include <functional>
#include <gsl/assert>
#include <limits>
#include <not_null.hpp>
#include <optional>
#include <range/v3/action/action.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>
#include <vector>

#include "core_common.h"
#include "core_sdb_tables.h"
#include "core_types.h"
#include "cpp_concepts.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "sqldb_alias_to_table.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_prm_db.h"
#include "sqldb_qb_common.h"
#include "sqldb_qb_condition.h"
#include "sqldb_qb_delete.h"
#include "sqldb_qb_insert.h"
#include "sqldb_qb_query_value.h"
#include "sqldb_qb_select.h"
#include "sqldb_qb_types.h"
#include "sqldb_qb_update.h"
#include "sqldb_query_builder.h"
#include "sqldb_rows.h"
#include "sqldb_value_common_conversions.h"  // IWYU pragma: keep
#include "sqldb_value_conversions.h"

namespace vh::stk::core {
namespace {
struct BaseAsset : public sqldb::AliasToTable<sdb::table::Asset, BaseAsset> {
  using id = AliasToColumn<Target::id>;
  using name = AliasToColumn<Target::name>;
  using base_asset = AliasToColumn<name, struct base_asset>;
};

struct QuoteAsset : public sqldb::AliasToTable<sdb::table::Asset, QuoteAsset> {
  using id = AliasToColumn<Target::id>;
  using name = AliasToColumn<Target::name>;
  using quote_asset = AliasToColumn<name, struct quote_asset>;
};

auto SelectSymbolsInfoBuilder [[nodiscard]] () {
  return sqldb::query_builder::Select<
             sdb::table::SymbolInfo::name,
             sdb::table::SymbolInfo::base_asset_min_amount,
             sdb::table::SymbolInfo::base_asset_price_step,
             sdb::table::SymbolInfo::quote_asset_min_amount,
             sdb::table::SymbolInfo::quote_asset_price_step,
             sqldb::qb::As<BaseAsset::name, BaseAsset::base_asset>,
             sqldb::qb::As<QuoteAsset::name, QuoteAsset::quote_asset>>()
      .From<sdb::table::SymbolInfo>()
      .Join<sqldb::qb::As<sdb::table::Asset, BaseAsset>>(sqldb::qb::On(
          sqldb::qb::Column<sdb::table::SymbolInfo::base_asset_id>() ==
          sqldb::qb::Column<BaseAsset::id>()))
      .Join<sqldb::qb::As<sdb::table::Asset, QuoteAsset>>(sqldb::qb::On(
          sqldb::qb::Column<sdb::table::SymbolInfo::quote_asset_id>() ==
          sqldb::qb::Column<QuoteAsset::id>()));
}

auto SymbolsInfoFrom [[nodiscard]] (sqldb::Rows rows) {
  auto &names = rows.GetColumnValues<sdb::table::SymbolInfo::name>();
  auto &base_assets = rows.GetColumnValues<BaseAsset::base_asset>();
  const auto &base_asset_min_amounts =
      rows.GetColumnValues<sdb::table::SymbolInfo::base_asset_min_amount>();
  const auto &base_asset_price_steps =
      rows.GetColumnValues<sdb::table::SymbolInfo::base_asset_price_step>();
  auto &quote_assets = rows.GetColumnValues<QuoteAsset::quote_asset>();
  const auto &quote_asset_min_amounts =
      rows.GetColumnValues<sdb::table::SymbolInfo::quote_asset_min_amount>();
  const auto &quote_asset_price_steps =
      rows.GetColumnValues<sdb::table::SymbolInfo::quote_asset_price_step>();

  const auto num_rows = rows.GetSize();
  auto infos = std::vector<SymbolInfo>{};
  infos.reserve(num_rows);

  for (auto i = 0; i < num_rows; ++i) {
    infos.emplace_back(SymbolInfo{
        .symbol = ValueAs<Symbol>(std::move(names[i])),
        .base_asset = {.asset = ValueAs<Asset>(std::move(base_assets[i])),
                       .min_amount = ValueAs<double>(base_asset_min_amounts[i]),
                       .price_step =
                           ValueAs<double>(base_asset_price_steps[i])},
        .quote_asset = {
            .asset = ValueAs<Asset>(std::move(quote_assets[i])),
            .min_amount = ValueAs<double>(quote_asset_min_amounts[i]),
            .price_step = ValueAs<double>(quote_asset_price_steps[i])}});
  }

  return infos;
}

template <typename T>
auto DefaultLess [[nodiscard]] (const T &left, const T &right) {
  return left < right;
}

template <typename T>
auto DefaultEquals [[nodiscard]] (const T &left, const T &right) {
  return left == right;
}

template <typename Item>
void UpdateItems(
    std::vector<Item> new_items,
    const cpp::CallableReturning<std::vector<Item>> auto &select_items,
    const cpp::VoidCallableTaking<const Item &> auto &insert_item,
    const cpp::VoidCallableTaking<const Item &> auto &update_item,
    const cpp::VoidCallableTaking<const Item &> auto &delete_item,
    const std::predicate<const Item &, const Item &> auto &item_less,
    const std::predicate<const Item &, const Item &> auto &item_equals) {
  const auto old_items = select_items() | ranges::actions::sort(item_less) |
                         ranges::actions::unique(item_equals);
  new_items |=
      ranges::actions::sort(item_less) | ranges::actions::unique(item_equals);

  const auto removed_items =
      ranges::views::set_difference(old_items, new_items, item_less);

  for (const auto &removed_item : removed_items) {
    delete_item(removed_item);
  }

  const auto added_items =
      ranges::views::set_difference(new_items, old_items, item_less);

  for (const auto &added_item : added_items) {
    insert_item(added_item);
  }

  const auto existing_items =
      ranges::views::set_intersection(new_items, old_items, item_less);
  const auto updated_items =
      ranges::views::set_difference(existing_items, old_items);

  for (const auto &updated_item : updated_items) {
    update_item(updated_item);
  }
}

auto SymbolInfoLess
    [[nodiscard]] (const SymbolInfo &left, const SymbolInfo &right) {
  return left.symbol < right.symbol;
}

auto SymbolInfoEquals
    [[nodiscard]] (const SymbolInfo &left, const SymbolInfo &right) {
  return left.symbol == right.symbol;
}
}  // namespace

SymbolsDb::SymbolsDb(cpp::meta::ThreadSafe<cpp::NnUp<sqldb::IDb>> sql_db)
    : prepared_statements_{[&sql_db]() {
        (*sql_db)->CreateTableIfNotExists<sdb::table::Asset>();
        (*sql_db)->CreateTableIfNotExists<sdb::table::SymbolInfo>();
        (*sql_db)->CreateTableIfNotExists<sdb::table::SymbolPriceRecord>();

        auto parametrized_db = sqldb::prm::Db{std::move(sql_db)};

        return PreparedStatements{
            .select_assets = parametrized_db.PrepareStatement(
                sqldb::query_builder::Select<sdb::table::Asset::name>()
                    .From<sdb::table::Asset>()
                    .Build()),

            .insert_asset = parametrized_db.PrepareStatement(
                sqldb::query_builder::Insert()
                    .Value<sdb::table::Asset::name>(
                        sqldb::qb::ParamForColumn<sdb::table::Asset::name>())
                    .Into<sdb::table::Asset>()
                    .Build()),

            .delete_asset = parametrized_db.PrepareStatement(
                sqldb::query_builder::DeleteFromTable<sdb::table::Asset>()
                    .Where(sqldb::qb::Column<sdb::table::Asset::name>() ==
                           sqldb::qb::ParamForColumn<sdb::table::Asset::name>())
                    .Build()),

            .select_symbols_with_price_records =
                parametrized_db.PrepareStatement(
                    sqldb::query_builder::Select<sdb::table::SymbolInfo::name>(
                        sqldb::qb::Distinct{})
                        .From<sdb::table::SymbolPriceRecord>()
                        .Join<sdb::table::SymbolInfo>(sqldb::qb::On(
                            sqldb::qb::Column<
                                sdb::table::SymbolPriceRecord::symbol_id>() ==
                            sqldb::qb::Column<sdb::table::SymbolInfo::id>()))
                        .Build()),

            .select_symbol_info = parametrized_db.PrepareStatement(
                SelectSymbolsInfoBuilder()
                    .Where(sqldb::qb::Column<sdb::table::SymbolInfo::name>() ==
                           sqldb::qb::ParamForColumn<
                               sdb::table::SymbolInfo::name>())
                    .Build()),

            .select_symbols_info = parametrized_db.PrepareStatement(
                SelectSymbolsInfoBuilder().Build()),

            .insert_symbol_info = parametrized_db.PrepareStatement(
                sqldb::query_builder::Insert()
                    .Value<sdb::table::SymbolInfo::name>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolInfo::name>())
                    .Value<sdb::table::SymbolInfo::base_asset_id>(
                        sqldb::query_builder::Select<sdb::table::Asset::id>()
                            .From<sdb::table::Asset>()
                            .Where(
                                sqldb::qb::Column<sdb::table::Asset::name>() ==
                                sqldb::qb::ParamForColumn<
                                    sdb::table::Asset::name>()))
                    .Value<sdb::table::SymbolInfo::base_asset_min_amount>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolInfo::base_asset_min_amount>())
                    .Value<sdb::table::SymbolInfo::base_asset_price_step>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolInfo::base_asset_price_step>())
                    .Value<sdb::table::SymbolInfo::quote_asset_id>(
                        sqldb::query_builder::Select<sdb::table::Asset::id>()
                            .From<sdb::table::Asset>()
                            .Where(
                                sqldb::qb::Column<sdb::table::Asset::name>() ==
                                sqldb::qb::ParamForColumn<
                                    sdb::table::Asset::name>()))
                    .Value<sdb::table::SymbolInfo::quote_asset_min_amount>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolInfo::quote_asset_min_amount>())
                    .Value<sdb::table::SymbolInfo::quote_asset_price_step>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolInfo::quote_asset_price_step>())
                    .Into<sdb::table::SymbolInfo>()
                    .Build()),

            .update_symbol_info = parametrized_db.PrepareStatement(
                sqldb::query_builder::UpdateTable<sdb::table::SymbolInfo>()
                    .Set<sdb::table::SymbolInfo::base_asset_id>(
                        sqldb::query_builder::Select<sdb::table::Asset::id>()
                            .From<sdb::table::Asset>()
                            .Where(
                                sqldb::qb::Column<sdb::table::Asset::name>() ==
                                sqldb::qb::ParamForColumn<
                                    sdb::table::Asset::name>()))
                    .Set<sdb::table::SymbolInfo::base_asset_min_amount>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolInfo::base_asset_min_amount>())
                    .Set<sdb::table::SymbolInfo::base_asset_price_step>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolInfo::base_asset_price_step>())
                    .Set<sdb::table::SymbolInfo::quote_asset_id>(
                        sqldb::query_builder::Select<sdb::table::Asset::id>()
                            .From<sdb::table::Asset>()
                            .Where(
                                sqldb::qb::Column<sdb::table::Asset::name>() ==
                                sqldb::qb::ParamForColumn<
                                    sdb::table::Asset::name>()))
                    .Set<sdb::table::SymbolInfo::quote_asset_min_amount>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolInfo::quote_asset_min_amount>())
                    .Set<sdb::table::SymbolInfo::quote_asset_price_step>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolInfo::quote_asset_price_step>())
                    .Where(sqldb::qb::Column<sdb::table::SymbolInfo::name>() ==
                           sqldb::qb::ParamForColumn<
                               sdb::table::SymbolInfo::name>())
                    .Build()),

            .delete_symbol_info = parametrized_db.PrepareStatement(
                sqldb::query_builder::DeleteFromTable<sdb::table::SymbolInfo>()
                    .Where(sqldb::qb::Column<sdb::table::SymbolInfo::name>() ==
                           sqldb::qb::ParamForColumn<
                               sdb::table::SymbolInfo::name>())
                    .Build()),

            .select_symbol_price_records =
                [&parametrized_db]() {
                  auto query_builder =
                      sqldb::query_builder::Select<
                          sdb::table::SymbolPriceRecord::buy_price,
                          sdb::table::SymbolPriceRecord::sell_price,
                          sdb::table::SymbolPriceRecord::time>()
                          .From<sdb::table::SymbolPriceRecord>()
                          .Where(
                              (sqldb::qb::Column<sdb::table::SymbolPriceRecord::
                                                     symbol_id>() ==
                               sqldb::query_builder::Select<
                                   sdb::table::SymbolInfo::id>()
                                   .From<sdb::table::SymbolInfo>()
                                   .Where(
                                       sqldb::qb::Column<
                                           sdb::table::SymbolInfo::name>() ==
                                       sqldb::qb::ParamForColumn<
                                           sdb::table::SymbolInfo::name>())) &&
                              (sqldb::qb::Column<
                                   sdb::table::SymbolPriceRecord::time>() >=
                               sqldb::qb::ParamForColumn<
                                   sdb::table::SymbolPriceRecord::time>()) &&
                              (sqldb::qb::Column<
                                   sdb::table::SymbolPriceRecord::time>() <
                               sqldb::qb::ParamForColumn<
                                   sdb::table::SymbolPriceRecord::time>()))
                          .Limit(sqldb::qb::ParamOfType<int>());

                  return PreparedStatements::SelectSymbolPriceRecords{
                      .order_by_time_ascending =
                          parametrized_db.PrepareStatement(
                              sqldb::qb::Select{query_builder}
                                  .OrderBy<sdb::table::SymbolPriceRecord::time>(
                                      sqldb::qb::Order::kAscending)
                                  .Build()),
                      .order_by_time_descending =
                          parametrized_db.PrepareStatement(
                              query_builder
                                  .OrderBy<sdb::table::SymbolPriceRecord::time>(
                                      sqldb::qb::Order::kDescending)
                                  .Build())};
                }(),

            .insert_symbol_price_record = parametrized_db.PrepareStatement(
                sqldb::query_builder::Insert()
                    .Value<sdb::table::SymbolPriceRecord::symbol_id>(
                        sqldb::query_builder::Select<
                            sdb::table::SymbolInfo::id>()
                            .From<sdb::table::SymbolInfo>()
                            .Where(sqldb::qb::Column<
                                       sdb::table::SymbolInfo::name>() ==
                                   sqldb::qb::ParamForColumn<
                                       sdb::table::SymbolInfo::name>()))
                    .Value<sdb::table::SymbolPriceRecord::buy_price>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolPriceRecord::buy_price>())
                    .Value<sdb::table::SymbolPriceRecord::sell_price>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolPriceRecord::sell_price>())
                    .Value<sdb::table::SymbolPriceRecord::time>(
                        sqldb::qb::ParamForColumn<
                            sdb::table::SymbolPriceRecord::time>())
                    .Into<sdb::table::SymbolPriceRecord>()
                    .Build()),

            .delete_symbol_price_records = parametrized_db.PrepareStatement(
                sqldb::query_builder::DeleteFromTable<
                    sdb::table::SymbolPriceRecord>()
                    .Where((sqldb::qb::Column<
                                sdb::table::SymbolPriceRecord::time>() >=
                            sqldb::qb::ParamForColumn<
                                sdb::table::SymbolPriceRecord::time>()) &&
                           (sqldb::qb::Column<
                                sdb::table::SymbolPriceRecord::time>() <
                            sqldb::qb::ParamForColumn<
                                sdb::table::SymbolPriceRecord::time>()))
                    .Build())};
      }()} {}

auto SymbolsDb::SelectAssetsImpl() const {
  auto rows = prepared_statements_.select_assets->Execute();
  auto &names = rows.GetColumnValues<sdb::table::Asset::name>();

  return names | ranges::views::transform([](auto &name) {
           return ValueAs<Asset>(std::move(name));
         }) |
         ranges::to_vector;
}

auto SymbolsDb::SelectAssets() const -> cppcoro::task<std::vector<Asset>> {
  co_return SelectAssetsImpl();
}

auto SymbolsDb::UpdateAssets(std::vector<Asset> assets) -> cppcoro::task<> {
  UpdateItems(
      std::move(assets), std::bind_front(&SymbolsDb::SelectAssetsImpl, this),
      std::bind_front(&SymbolsDb::InsertAsset, this), [](const auto &) {},
      std::bind_front(&SymbolsDb::DeleteAsset, this), &DefaultLess<Asset>,
      &DefaultEquals<Asset>);
  co_return;
}

auto SymbolsDb::SelectSymbolsWithPriceRecords() const
    -> cppcoro::task<std::vector<Symbol>> {
  auto rows = prepared_statements_.select_symbols_with_price_records->Execute();
  auto &names = rows.GetColumnValues<sdb::table::SymbolInfo::name>();

  co_return names | ranges::views::transform([](auto &name) {
    return ValueAs<Symbol>(std::move(name));
  }) | ranges::to_vector;
}

auto SymbolsDb::SelectSymbolInfo(Symbol symbol) const
    -> cppcoro::task<cpp::Opt<SymbolInfo>> {
  auto rows = prepared_statements_.select_symbol_info->Execute(
      sqldb::AsValues(std::move(symbol)));
  auto infos = SymbolsInfoFrom(std::move(rows));

  if (infos.empty()) {
    co_return std::nullopt;
  }

  Expects(infos.size() == 1);
  co_return std::move(infos.front());
}

auto SymbolsDb::SelectSymbolsInfoImpl() const {
  auto rows = prepared_statements_.select_symbols_info->Execute();
  return SymbolsInfoFrom(std::move(rows));
}

auto SymbolsDb::SelectSymbolsInfo() const
    -> cppcoro::task<std::vector<SymbolInfo>> {
  co_return SelectSymbolsInfoImpl();
}

auto SymbolsDb::UpdateSymbolsInfo(std::vector<SymbolInfo> infos)
    -> cppcoro::task<> {
  UpdateItems(std::move(infos),
              std::bind_front(&SymbolsDb::SelectSymbolsInfoImpl, this),
              std::bind_front(&SymbolsDb::InsertSymbolInfo, this),
              std::bind_front(&SymbolsDb::UpdateSymbolInfo, this),
              std::bind_front(&SymbolsDb::DeleteSymbolInfo, this),
              &SymbolInfoLess, &SymbolInfoEquals);
  co_return;
}

auto SymbolsDb::SelectSymbolPriceRecords(const Symbol &symbol,
                                         const cpp::Opt<TimeOrder> &order,
                                         const cpp::Opt<absl::Time> &start_time,
                                         const cpp::Opt<absl::Time> &end_time,
                                         const cpp::Opt<int> &limit) const
    -> cppcoro::task<std::vector<SymbolPriceRecord>> {
  const auto &statement = (TimeOrderFrom(order) == TimeOrder::kNewFirst)
                              ? prepared_statements_.select_symbol_price_records
                                    .order_by_time_descending
                              : prepared_statements_.select_symbol_price_records
                                    .order_by_time_ascending;

  const auto rows = statement->Execute(
      sqldb::AsValues(symbol, StartTimeFrom(start_time), EndTimeFrom(end_time),
                      limit.value_or(std::numeric_limits<int>::max())));

  const auto &buy_prices =
      rows.GetColumnValues<sdb::table::SymbolPriceRecord::buy_price>();
  const auto &sell_prices =
      rows.GetColumnValues<sdb::table::SymbolPriceRecord::sell_price>();
  const auto &times =
      rows.GetColumnValues<sdb::table::SymbolPriceRecord::time>();

  const auto num_rows = rows.GetSize();
  auto price_ticks = std::vector<SymbolPriceRecord>{};
  price_ticks.reserve(num_rows);

  for (auto i = 0; i < num_rows; ++i) {
    price_ticks.emplace_back(
        SymbolPriceRecord{.symbol = symbol,
                          .buy_price = ValueAs<Price>(buy_prices[i]),
                          .sell_price = ValueAs<Price>(sell_prices[i]),
                          .time = ValueAs<absl::Time>(times[i])});
  }

  co_return price_ticks;
}

auto SymbolsDb::InsertSymbolPriceRecord(SymbolPriceRecord record)
    -> cppcoro::task<> {
  prepared_statements_.insert_symbol_price_record->Execute(
      sqldb::AsValues(std::move(record.symbol), record.buy_price,
                      record.sell_price, record.time));
  co_return;
}

auto SymbolsDb::DeleteSymbolPriceRecords(const cpp::Opt<absl::Time> &start_time,
                                         const cpp::Opt<absl::Time> &end_time)
    -> cppcoro::task<> {
  prepared_statements_.delete_symbol_price_records->Execute(
      sqldb::AsValues(StartTimeFrom(start_time), EndTimeFrom(end_time)));
  co_return;
}

void SymbolsDb::InsertAsset(Asset asset) const {
  prepared_statements_.insert_asset->Execute(sqldb::AsValues(std::move(asset)));
}

void SymbolsDb::DeleteAsset(Asset asset) const {
  prepared_statements_.delete_asset->Execute(sqldb::AsValues(std::move(asset)));
}

void SymbolsDb::InsertSymbolInfo(SymbolInfo info) const {
  prepared_statements_.insert_symbol_info->Execute(sqldb::AsValues(
      std::move(info.symbol), std::move(info.base_asset.asset),
      info.base_asset.min_amount, info.base_asset.price_step,
      std::move(info.quote_asset.asset), info.quote_asset.min_amount,
      info.quote_asset.price_step));
}

void SymbolsDb::UpdateSymbolInfo(SymbolInfo info) const {
  prepared_statements_.update_symbol_info->Execute(sqldb::AsValues(
      std::move(info.base_asset.asset), info.base_asset.min_amount,
      info.base_asset.price_step, std::move(info.quote_asset.asset),
      info.quote_asset.min_amount, info.quote_asset.price_step,
      std::move(info.symbol)));
}

void SymbolsDb::DeleteSymbolInfo(SymbolInfo info) const {
  prepared_statements_.delete_symbol_info->Execute(
      sqldb::AsValues(std::move(info.symbol)));
}
}  // namespace vh::stk::core