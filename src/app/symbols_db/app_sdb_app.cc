#include "app_sdb_app.h"

#include <compare>
#include <functional>
#include <memory>
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

#include "app_sdb_prepared_statements.h"
#include "app_sdb_tables.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "cpp_views.h"
#include "not_null.hpp"
#include "sqldb_as_values.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_rows.h"
#include "sqldb_types.h"

namespace stonks::app::sdb {
namespace {
template <typename Item>
void UpdateItems(
    std::vector<Item> new_items,
    const cpp::InvocableReturning<std::vector<Item>> auto &select_items,
    const cpp::VoidInvocableTaking<const Item &> auto &insert_item,
    const cpp::VoidInvocableTaking<const Item &> auto &update_item,
    const cpp::VoidInvocableTaking<const Item &> auto &delete_item) {
  const auto old_items =
      select_items() | ranges::actions::sort | ranges::actions::unique;
  new_items |= ranges::actions::sort | ranges::actions::unique;

  const auto removed_items =
      ranges::views::set_difference(old_items, new_items);

  for (const auto &removed_item : removed_items) {
    delete_item(removed_item);
  }

  const auto added_items = ranges::views::set_difference(new_items, old_items);

  for (const auto &added_item : added_items) {
    insert_item(added_item);
  }

  const auto existing_items =
      ranges::views::set_intersection(new_items, old_items);
  const auto updated_items =
      ranges::views::set_difference(existing_items, old_items);

  for (const auto &updated_item : updated_items) {
    update_item(updated_item);
  }
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
         cpp::NnUp<sqldb::IQueryBuilder> query_builder,
         PreparedStatements prepared_statements)
    : db_{std::move(db)},
      query_builder_{std::move(query_builder)},
      prepared_statements_{std::move(prepared_statements)} {
  CreateTablesIfNotExist();
}

auto App::SelectAssets() const {
  auto rows = prepared_statements_.SelectAssets().Execute();
  auto &name_values = rows.GetColumnValues({tables::Asset::kName});
  return name_values | ranges::views::transform([](auto &asset) {
           return core::Asset{std::move(asset.GetString())};
         }) |
         ranges::to_vector;
}

void App::UpdateAssets(std::vector<core::Asset> assets) {
  UpdateItems(
      std::move(assets), std::bind_front(&App::SelectAssets, this),
      std::bind_front(&App::InsertAsset, this), [](const auto &) {},
      std::bind_front(&App::DeleteAsset, this));
}

void App::InsertAsset(core::Asset asset) {
  prepared_statements_.InsertAsset().Execute(sqldb::AsValues(std::move(asset)));
}

void App::DeleteAsset(core::Asset asset) {
  prepared_statements_.DeleteAsset().Execute(sqldb::AsValues(std::move(asset)));
}
}  // namespace stonks::app::sdb