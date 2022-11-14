#include "core_symbols_db.h"

#include <absl/time/time.h>
#include <stdint.h>

#include <compare>
#include <functional>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <optional>
#include <range/v3/action/action.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>
#include <vector>

#include "core_sdb_tables.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "parametrized/sqldb_p_db.h"
#include "sqldb_alias_to_table.h"
#include "sqldb_as_values.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
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
#include "sqldb_value.h"

namespace stonks::core {
namespace {
struct BaseAsset : public sqldb::AliasToTable<sdb::tables::Asset, BaseAsset> {
  using id = AliasToColumn<Target::id>;
  using name = AliasToColumn<Target::name>;
  using base_asset = AliasToColumn<name, struct base_asset>;
};

struct QuoteAsset : public sqldb::AliasToTable<sdb::tables::Asset, QuoteAsset> {
  using id = AliasToColumn<Target::id>;
  using name = AliasToColumn<Target::name>;
  using quote_asset = AliasToColumn<name, struct quote_asset>;
};

auto SelectSymbolsInfoBuilder [[nodiscard]] () {
  return sqldb::query_builder::Select<
             sdb::tables::SymbolInfo::name,
             sdb::tables::SymbolInfo::base_asset_min_amount,
             sdb::tables::SymbolInfo::base_asset_price_step,
             sdb::tables::SymbolInfo::quote_asset_min_amount,
             sdb::tables::SymbolInfo::quote_asset_price_step,
             sqldb::qb::As<BaseAsset::name, BaseAsset::base_asset>,
             sqldb::qb::As<QuoteAsset::name, QuoteAsset::quote_asset>>()
      .From<sdb::tables::SymbolInfo>()
      .Join<sqldb::qb::As<sdb::tables::Asset, BaseAsset>>(sqldb::qb::On(
          sqldb::qb::Column<sdb::tables::SymbolInfo::base_asset_id>() ==
          sqldb::qb::Column<BaseAsset::id>()))
      .Join<sqldb::qb::As<sdb::tables::Asset, QuoteAsset>>(sqldb::qb::On(
          sqldb::qb::Column<sdb::tables::SymbolInfo::quote_asset_id>() ==
          sqldb::qb::Column<QuoteAsset::id>()));
}

auto SymbolsInfoFrom [[nodiscard]] (sqldb::Rows rows) {
  auto &names = rows.GetColumnValues<sdb::tables::SymbolInfo::name>();
  auto &base_assets = rows.GetColumnValues<BaseAsset::base_asset>();
  const auto &base_asset_min_amounts =
      rows.GetColumnValues<sdb::tables::SymbolInfo::base_asset_min_amount>();
  const auto &base_asset_price_steps =
      rows.GetColumnValues<sdb::tables::SymbolInfo::base_asset_price_step>();
  auto &quote_assets = rows.GetColumnValues<QuoteAsset::quote_asset>();
  const auto &quote_asset_min_amounts =
      rows.GetColumnValues<sdb::tables::SymbolInfo::quote_asset_min_amount>();
  const auto &quote_asset_price_steps =
      rows.GetColumnValues<sdb::tables::SymbolInfo::quote_asset_price_step>();

  const auto num_rows = rows.GetSize();
  auto infos = std::vector<SymbolInfo>{};
  infos.reserve(num_rows);

  for (auto i = 0; i < num_rows; ++i) {
    infos.emplace_back(SymbolInfo{
        .symbol = {std::move(names[i].Get<std::string>())},
        .base_asset = {.asset = {std::move(base_assets[i].Get<std::string>())},
                       .min_amount = base_asset_min_amounts[i].Get<double>(),
                       .price_step = base_asset_price_steps[i].Get<double>()},
        .quote_asset = {
            .asset = {std::move(quote_assets[i].Get<std::string>())},
            .min_amount = quote_asset_min_amounts[i].Get<double>(),
            .price_step = quote_asset_price_steps[i].Get<double>()}});
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
    const cpp::InvocableReturning<std::vector<Item>> auto &select_items,
    const cpp::VoidInvocableTaking<const Item &> auto &insert_item,
    const cpp::VoidInvocableTaking<const Item &> auto &update_item,
    const cpp::VoidInvocableTaking<const Item &> auto &delete_item,
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

SymbolsDb::SymbolsDb(cpp::NnUp<sqldb::IDb> db)
    : db_{cpp::MakeNnSp<sqldb::p::Db>(std::move(db))} {
  auto &inner_db = db_->GetDb();
  inner_db.CreateTableIfNotExists<sdb::tables::Asset>();
  inner_db.CreateTableIfNotExists<sdb::tables::SymbolInfo>();
  inner_db.CreateTableIfNotExists<sdb::tables::SymbolPriceRecord>();
}

auto SymbolsDb::SelectAssets() const -> std::vector<Asset> {
  auto &statement = prepared_statements_.select_assets;

  if (statement == nullptr) {
    statement = db_->PrepareStatement(
                       sqldb::query_builder::Select<sdb::tables::Asset::name>()
                           .From<sdb::tables::Asset>()
                           .Build())
                    .as_nullable();
  }

  auto rows = statement->Execute();
  auto &names = rows.GetColumnValues<sdb::tables::Asset::name>();

  Ensures(statement);
  return names | ranges::views::transform([](auto &name) {
           return Asset{std::move(name.template Get<std::string>())};
         }) |
         ranges::to_vector;
}

void SymbolsDb::UpdateAssets(std::vector<Asset> assets) {
  UpdateItems(
      std::move(assets), std::bind_front(&SymbolsDb::SelectAssets, this),
      std::bind_front(&SymbolsDb::InsertAsset, this), [](const auto &) {},
      std::bind_front(&SymbolsDb::DeleteAsset, this), &DefaultLess<Asset>,
      &DefaultEquals<Asset>);
}

auto SymbolsDb::SelectSymbolsWithPriceRecords() const -> std::vector<Symbol> {
  auto &statement = prepared_statements_.select_symbols_with_price_records;

  if (statement == nullptr) {
    statement =
        db_->PrepareStatement(
               sqldb::query_builder::Select<sdb::tables::SymbolInfo::name>()
                   .From<sdb::tables::SymbolInfo>()
                   .Where(sqldb::qb::Exists(
                       sqldb::query_builder::SelectOne()
                           .From<sdb::tables::SymbolPriceRecord>()
                           .Where(
                               sqldb::qb::Column<
                                   sdb::tables::SymbolPriceRecord::
                                       symbol_id>() ==
                               sqldb::qb::Column<sdb::tables::SymbolInfo::id>())
                           .Limit(sqldb::Value{1})))
                   .Build())
            .as_nullable();
  }

  auto rows = statement->Execute();
  auto &names = rows.GetColumnValues<sdb::tables::SymbolInfo::name>();

  Ensures(statement);
  return names | ranges::views::transform([](auto &name) {
           return Symbol{std::move(name.template Get<std::string>())};
         }) |
         ranges::to_vector;
}

auto SymbolsDb::SelectSymbolInfo(Symbol symbol) const -> cpp::Opt<SymbolInfo> {
  auto &statement = prepared_statements_.select_symbol_info;

  if (statement == nullptr) {
    statement =
        db_->PrepareStatement(
               SelectSymbolsInfoBuilder()
                   .Where(sqldb::qb::Column<sdb::tables::SymbolInfo::name>() ==
                          sqldb::qb::ParamForColumn<
                              sdb::tables::SymbolInfo::name>())
                   .Build())
            .as_nullable();
  }

  auto rows = statement->Execute(sqldb::AsValues(std::move(symbol)));
  auto infos = SymbolsInfoFrom(std::move(rows));

  if (infos.empty()) {
    return std::nullopt;
  }

  Expects(infos.size() == 1);

  Ensures(statement);
  return std::move(infos.front());
}

auto SymbolsDb::SelectSymbolsInfo() const -> std::vector<SymbolInfo> {
  auto &statement = prepared_statements_.select_symbols_info;

  if (statement == nullptr) {
    statement =
        db_->PrepareStatement(SelectSymbolsInfoBuilder().Build()).as_nullable();
  }

  auto rows = statement->Execute();

  Ensures(statement);
  return SymbolsInfoFrom(std::move(rows));
}

void SymbolsDb::UpdateSymbolsInfo(std::vector<SymbolInfo> infos) {
  UpdateItems(std::move(infos),
              std::bind_front(&SymbolsDb::SelectSymbolsInfo, this),
              std::bind_front(&SymbolsDb::InsertSymbolInfo, this),
              std::bind_front(&SymbolsDb::UpdateSymbolInfo, this),
              std::bind_front(&SymbolsDb::DeleteSymbolInfo, this),
              &SymbolInfoLess, &SymbolInfoEquals);
}

auto SymbolsDb::SelectSymbolPriceRecords(
    const SelectSymbolPriceRecordsArgs &args) const
    -> std::vector<SymbolPriceRecord> {
  auto &statement = prepared_statements_.select_symbol_price_records;

  if (statement == nullptr) {
    statement =
        db_->PrepareStatement(
               sqldb::query_builder::Select<
                   sdb::tables::SymbolPriceRecord::price,
                   sdb::tables::SymbolPriceRecord::time>()
                   .From<sdb::tables::SymbolPriceRecord>()
                   .Where((sqldb::qb::Column<
                               sdb::tables::SymbolPriceRecord::symbol_id>() ==
                           sqldb::query_builder::Select<
                               sdb::tables::SymbolInfo::id>()
                               .From<sdb::tables::SymbolInfo>()
                               .Where(sqldb::qb::Column<
                                          sdb::tables::SymbolInfo::name>() ==
                                      sqldb::qb::ParamForColumn<
                                          sdb::tables::SymbolInfo::name>())) &&
                          (sqldb::qb::Column<
                               sdb::tables::SymbolPriceRecord::time>() >=
                           sqldb::qb::ParamForColumn<
                               sdb::tables::SymbolPriceRecord::time>()) &&
                          (sqldb::qb::Column<
                               sdb::tables::SymbolPriceRecord::time>() <
                           sqldb::qb::ParamForColumn<
                               sdb::tables::SymbolPriceRecord::time>()))
                   .Build())
            .as_nullable();
  }

  const auto rows = statement->Execute(
      sqldb::AsValues(args.symbol, absl::ToUnixMillis(args.start_time),
                      absl::ToUnixMillis(args.end_time) /*, args.limit*/));

  const auto &prices =
      rows.GetColumnValues<sdb::tables::SymbolPriceRecord::price>();
  const auto &times =
      rows.GetColumnValues<sdb::tables::SymbolPriceRecord::time>();

  const auto num_rows = rows.GetSize();
  auto price_ticks = std::vector<SymbolPriceRecord>{};
  price_ticks.reserve(num_rows);

  for (auto i = 0; i < num_rows; ++i) {
    price_ticks.emplace_back(SymbolPriceRecord{
        .symbol = args.symbol,
        .price = Price{prices[i].Get<double>()},
        .time = absl::FromUnixMillis(times[i].Get<int64_t>())});
  }

  Ensures(statement);
  return price_ticks;
}

void SymbolsDb::InsertSymbolPriceRecord(SymbolPriceRecord record) {
  auto &statement = prepared_statements_.insert_symbol_price_record;

  if (statement == nullptr) {
    statement =
        db_->PrepareStatement(
               sqldb::query_builder::Insert()
                   .Value<sdb::tables::SymbolPriceRecord::symbol_id>(
                       sqldb::query_builder::Select<
                           sdb::tables::SymbolInfo::id>()
                           .From<sdb::tables::SymbolInfo>()
                           .Where(sqldb::qb::Column<
                                      sdb::tables::SymbolInfo::name>() ==
                                  sqldb::qb::ParamForColumn<
                                      sdb::tables::SymbolInfo::name>()))
                   .Value<sdb::tables::SymbolPriceRecord::price>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolPriceRecord::price>())
                   .Value<sdb::tables::SymbolPriceRecord::time>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolPriceRecord::time>())
                   .Into<sdb::tables::SymbolPriceRecord>()
                   .Build())
            .as_nullable();
  }

  statement->Execute(sqldb::AsValues(std::move(record.symbol), record.price,
                                     absl::ToUnixMillis(record.time)));
  Ensures(statement);
}

void SymbolsDb::DeleteSymbolPriceRecords(absl::Time before_time) {
  auto &statement = prepared_statements_.delete_symbol_price_records;

  if (statement == nullptr) {
    statement = db_->PrepareStatement(
                       sqldb::query_builder::DeleteFromTable<
                           sdb::tables::SymbolPriceRecord>()
                           .Where(sqldb::qb::Column<
                                      sdb::tables::SymbolPriceRecord::time>() <
                                  sqldb::qb::ParamForColumn<
                                      sdb::tables::SymbolPriceRecord::time>())
                           .Build())
                    .as_nullable();
  }

  statement->Execute(sqldb::AsValues(absl::ToUnixMillis(before_time)));
  Ensures(statement);
}

void SymbolsDb::InsertAsset(Asset asset) {
  auto &statement = prepared_statements_.insert_asset;

  if (statement == nullptr) {
    statement =
        db_->PrepareStatement(
               sqldb::query_builder::Insert()
                   .Value<sdb::tables::Asset::name>(
                       sqldb::qb::ParamForColumn<sdb::tables::Asset::name>())
                   .Into<sdb::tables::Asset>()
                   .Build())
            .as_nullable();
  }

  statement->Execute(sqldb::AsValues(std::move(asset)));
  Ensures(statement);
}

void SymbolsDb::DeleteAsset(Asset asset) {
  auto &statement = prepared_statements_.delete_asset;

  if (statement == nullptr) {
    statement =
        db_->PrepareStatement(
               sqldb::query_builder::DeleteFromTable<sdb::tables::Asset>()
                   .Where(sqldb::qb::Column<sdb::tables::Asset::name>() ==
                          sqldb::qb::ParamForColumn<sdb::tables::Asset::name>())
                   .Build())
            .as_nullable();
  }

  statement->Execute(sqldb::AsValues(std::move(asset)));
  Ensures(statement);
}

void SymbolsDb::InsertSymbolInfo(SymbolInfo info) {
  auto &statement = prepared_statements_.insert_symbol_info;

  if (statement == nullptr) {
    statement =
        db_->PrepareStatement(
               sqldb::query_builder::Insert()
                   .Value<sdb::tables::SymbolInfo::name>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolInfo::name>())
                   .Value<sdb::tables::SymbolInfo::base_asset_id>(
                       sqldb::query_builder::Select<sdb::tables::Asset::id>()
                           .From<sdb::tables::Asset>()
                           .Where(
                               sqldb::qb::Column<sdb::tables::Asset::name>() ==
                               sqldb::qb::ParamForColumn<
                                   sdb::tables::Asset::name>()))
                   .Value<sdb::tables::SymbolInfo::base_asset_min_amount>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolInfo::base_asset_min_amount>())
                   .Value<sdb::tables::SymbolInfo::base_asset_price_step>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolInfo::base_asset_price_step>())
                   .Value<sdb::tables::SymbolInfo::quote_asset_id>(
                       sqldb::query_builder::Select<sdb::tables::Asset::id>()
                           .From<sdb::tables::Asset>()
                           .Where(
                               sqldb::qb::Column<sdb::tables::Asset::name>() ==
                               sqldb::qb::ParamForColumn<
                                   sdb::tables::Asset::name>()))
                   .Value<sdb::tables::SymbolInfo::quote_asset_min_amount>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolInfo::quote_asset_min_amount>())
                   .Value<sdb::tables::SymbolInfo::quote_asset_price_step>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolInfo::quote_asset_price_step>())
                   .Into<sdb::tables::SymbolInfo>()
                   .Build())
            .as_nullable();
  }

  statement->Execute(sqldb::AsValues(
      std::move(info.symbol), std::move(info.base_asset.asset),
      info.base_asset.min_amount, info.base_asset.price_step,
      std::move(info.quote_asset.asset), info.quote_asset.min_amount,
      info.quote_asset.price_step));
  Ensures(statement);
}

void SymbolsDb::UpdateSymbolInfo(SymbolInfo info) {
  auto &statement = prepared_statements_.update_symbol_info;

  if (statement == nullptr) {
    statement =
        db_->PrepareStatement(
               sqldb::query_builder::UpdateTable<sdb::tables::SymbolInfo>()
                   .Set<sdb::tables::SymbolInfo::base_asset_id>(
                       sqldb::query_builder::Select<sdb::tables::Asset::id>()
                           .From<sdb::tables::Asset>()
                           .Where(
                               sqldb::qb::Column<sdb::tables::Asset::name>() ==
                               sqldb::qb::ParamForColumn<
                                   sdb::tables::Asset::name>()))
                   .Set<sdb::tables::SymbolInfo::base_asset_min_amount>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolInfo::base_asset_min_amount>())
                   .Set<sdb::tables::SymbolInfo::base_asset_price_step>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolInfo::base_asset_price_step>())
                   .Set<sdb::tables::SymbolInfo::quote_asset_id>(
                       sqldb::query_builder::Select<sdb::tables::Asset::id>()
                           .From<sdb::tables::Asset>()
                           .Where(
                               sqldb::qb::Column<sdb::tables::Asset::name>() ==
                               sqldb::qb::ParamForColumn<
                                   sdb::tables::Asset::name>()))
                   .Set<sdb::tables::SymbolInfo::quote_asset_min_amount>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolInfo::quote_asset_min_amount>())
                   .Set<sdb::tables::SymbolInfo::quote_asset_price_step>(
                       sqldb::qb::ParamForColumn<
                           sdb::tables::SymbolInfo::quote_asset_price_step>())
                   .Where(sqldb::qb::Column<sdb::tables::SymbolInfo::name>() ==
                          sqldb::qb::ParamForColumn<
                              sdb::tables::SymbolInfo::name>())
                   .Build())
            .as_nullable();
  }

  statement->Execute(sqldb::AsValues(
      std::move(info.base_asset.asset), info.base_asset.min_amount,
      info.base_asset.price_step, std::move(info.quote_asset.asset),
      info.quote_asset.min_amount, info.quote_asset.price_step,
      std::move(info.symbol)));
  Ensures(statement);
}

void SymbolsDb::DeleteSymbolInfo(SymbolInfo info) {
  auto &statement = prepared_statements_.delete_symbol_info;

  if (statement == nullptr) {
    statement =
        db_->PrepareStatement(
               sqldb::query_builder::DeleteFromTable<sdb::tables::SymbolInfo>()
                   .Where(sqldb::qb::Column<sdb::tables::SymbolInfo::name>() ==
                          sqldb::qb::ParamForColumn<
                              sdb::tables::SymbolInfo::name>())
                   .Build())
            .as_nullable();
  }

  statement->Execute(sqldb::AsValues(std::move(info.symbol)));
  Ensures(statement);
}
}  // namespace stonks::core