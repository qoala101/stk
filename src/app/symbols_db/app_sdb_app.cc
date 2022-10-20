#include "app_sdb_app.h"

#include <absl/time/time.h>

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

#include "app_sdb_prepared_statements.h"
#include "app_sdb_tables.h"
#include "core_types.h"
#include "cpp_lazy.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "cpp_views.h"
#include "sqldb_as_values.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_rows.h"
#include "sqldb_types.h"
#include "sqldb_value.h"

namespace stonks::app::sdb {
namespace {
template <typename T>
[[nodiscard]] auto DefaultLess(const T &left, const T &right) {
  return left < right;
}

template <typename T>
[[nodiscard]] auto DefaultEquals(const T &left, const T &right) {
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

[[nodiscard]] auto SymbolInfoLess(const core::SymbolInfo &left,
                                  const core::SymbolInfo &right) {
  return left.symbol < right.symbol;
}

[[nodiscard]] auto SymbolInfoEquals(const core::SymbolInfo &left,
                                    const core::SymbolInfo &right) {
  return left.symbol == right.symbol;
}

[[nodiscard]] auto SymbolsInfoFrom(sqldb::Rows rows) {
  auto &names = rows.GetColumnValues({tables::SymbolInfo::kName});
  auto &base_assets = rows.GetColumnValues({"base_asset"});
  const auto &base_asset_min_amounts =
      rows.GetColumnValues({tables::SymbolInfo::kBaseAssetMinAmount});
  const auto &base_asset_price_steps =
      rows.GetColumnValues({tables::SymbolInfo::kBaseAssetPriceStep});
  auto &quote_assets = rows.GetColumnValues({"quote_asset"});
  const auto &quote_asset_min_amounts =
      rows.GetColumnValues({tables::SymbolInfo::kQuoteAssetMinAmount});
  const auto &quote_asset_price_steps =
      rows.GetColumnValues({tables::SymbolInfo::kQuoteAssetPriceStep});

  const auto num_rows = rows.GetSize();
  auto infos = std::vector<core::SymbolInfo>{};
  infos.reserve(num_rows);

  for (auto i = 0; i < num_rows; ++i) {
    infos.emplace_back(core::SymbolInfo{
        .symbol = {std::move(names[i].GetString())},
        .base_asset = {.asset = {std::move(base_assets[i].GetString())},
                       .min_amount = base_asset_min_amounts[i].GetDouble(),
                       .price_step = base_asset_price_steps[i].GetDouble()},
        .quote_asset = {.asset = {std::move(quote_assets[i].GetString())},
                        .min_amount = quote_asset_min_amounts[i].GetDouble(),
                        .price_step = quote_asset_price_steps[i].GetDouble()}});
  }

  return infos;
}
}  // namespace

void App::CreateTablesIfNotExist() {
  const auto tables = cpp::ConstView<sqldb::TableDefinition>{
      cpp::AssumeNn(&tables::Asset::Definition()),
      cpp::AssumeNn(&tables::SymbolInfo::Definition()),
      cpp::AssumeNn(&tables::SymbolPriceRecord::Definition())};

  for (const auto table : tables) {
    db_->PrepareStatement(
           query_builder_->BuildCreateTableIfNotExistsQuery(*table))
        ->Execute();
  }
}

App::App(cpp::NnUp<sqldb::IDb> db,
         cpp::NnUp<sqldb::IQueryBuilder> query_builder)
    : db_{std::move(db)},
      query_builder_{std::move(query_builder)},
      prepared_statements_{PreparedStatementsFrom(db_)} {
  CreateTablesIfNotExist();
}

auto App::SelectAssets() const -> std::vector<core::Asset> {
  auto rows = prepared_statements_.select_assets->Execute();
  auto &names = rows.GetColumnValues({tables::Asset::kName});
  return names | ranges::views::transform([](sqldb::Value &name) {
           return core::Asset{std::move(name.GetString())};
         }) |
         ranges::to_vector;
}

void App::UpdateAssets(std::vector<core::Asset> assets) {
  UpdateItems(
      std::move(assets), std::bind_front(&App::SelectAssets, this),
      std::bind_front(&App::InsertAsset, this), [](const core::Asset &) {},
      std::bind_front(&App::DeleteAsset, this), &DefaultLess<core::Asset>,
      &DefaultEquals<core::Asset>);
}

auto App::SelectSymbolsWithPriceRecords() const -> std::vector<core::Symbol> {
  auto rows = prepared_statements_.select_symbols_with_price_records->Execute();
  auto &names = rows.GetColumnValues({tables::SymbolInfo::kName});
  return names | ranges::views::transform([](sqldb::Value &name) {
           return core::Symbol{std::move(name.GetString())};
         }) |
         ranges::to_vector;
}

auto App::SelectSymbolInfo(core::Symbol symbol) const
    -> cpp::Opt<core::SymbolInfo> {
  auto rows = prepared_statements_.select_symbol_info->Execute(
      sqldb::AsValues(std::move(symbol)));
  auto infos = SymbolsInfoFrom(std::move(rows));

  if (infos.empty()) {
    return std::nullopt;
  }

  Expects(infos.size() == 1);
  return std::move(infos.front());
}

auto App::SelectSymbolsInfo() const -> std::vector<core::SymbolInfo> {
  auto rows = prepared_statements_.select_symbols_info->Execute();
  return SymbolsInfoFrom(std::move(rows));
}

void App::UpdateSymbolsInfo(std::vector<core::SymbolInfo> infos) {
  UpdateItems(std::move(infos), std::bind_front(&App::SelectSymbolsInfo, this),
              std::bind_front(&App::InsertSymbolInfo, this),
              std::bind_front(&App::UpdateSymbolInfo, this),
              std::bind_front(&App::DeleteSymbolInfo, this), &SymbolInfoLess,
              &SymbolInfoEquals);
}

auto App::SelectSymbolPriceRecords(const SelectSymbolPriceRecordsArgs &args)
    const -> std::vector<core::SymbolPriceRecord> {
  const auto rows = prepared_statements_.select_symbol_price_records->Execute(
      sqldb::AsValues(args.symbol, absl::ToUnixMillis(args.start_time),
                      absl::ToUnixMillis(args.end_time)));

  const auto &prices =
      rows.GetColumnValues({tables::SymbolPriceRecord::kPrice});
  const auto &times = rows.GetColumnValues({tables::SymbolPriceRecord::kTime});

  const auto num_rows = rows.GetSize();
  auto price_ticks = std::vector<core::SymbolPriceRecord>{};
  price_ticks.reserve(num_rows);

  for (auto i = 0; i < num_rows; ++i) {
    price_ticks.emplace_back(core::SymbolPriceRecord{
        .symbol = args.symbol,
        .price = core::Price{prices[i].GetDouble()},
        .time = absl::FromUnixMillis(times[i].GetInt64())});
  }

  return price_ticks;
}

void App::InsertSymbolPriceRecord(core::SymbolPriceRecord record) {
  prepared_statements_.insert_symbol_price_record->Execute(sqldb::AsValues(
      std::move(record.symbol), record.price, absl::ToUnixMillis(record.time)));
}

void App::DeleteSymbolPriceRecords(absl::Time before_time) {
  prepared_statements_.delete_symbol_price_records->Execute(
      sqldb::AsValues(absl::ToUnixMillis(before_time)));
}

void App::InsertAsset(core::Asset asset) {
  prepared_statements_.insert_asset->Execute(sqldb::AsValues(std::move(asset)));
}

void App::DeleteAsset(core::Asset asset) {
  prepared_statements_.delete_asset->Execute(sqldb::AsValues(std::move(asset)));
}

void App::InsertSymbolInfo(core::SymbolInfo info) {
  prepared_statements_.insert_symbol_info->Execute(sqldb::AsValues(
      std::move(info.symbol), std::move(info.base_asset.asset),
      info.base_asset.min_amount, info.base_asset.price_step,
      std::move(info.quote_asset.asset), info.quote_asset.min_amount,
      info.quote_asset.price_step));
}

void App::UpdateSymbolInfo(core::SymbolInfo info) {
  prepared_statements_.update_symbol_info->Execute(sqldb::AsValues(
      std::move(info.base_asset.asset), info.base_asset.min_amount,
      info.base_asset.price_step, std::move(info.quote_asset.asset),
      info.quote_asset.min_amount, info.quote_asset.price_step,
      std::move(info.symbol)));
}

void App::DeleteSymbolInfo(core::SymbolInfo info) {
  prepared_statements_.delete_symbol_info->Execute(
      sqldb::AsValues(std::move(info.symbol)));
}
}  // namespace stonks::app::sdb