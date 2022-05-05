#include "finance_db.h"

#include <spdlog/spdlog.h>

#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/transform.hpp>

#include "finance_api.h"
#include "finance_db_table_definitions.h"
#include "null_db.h"
#include "sqlite_db.h"

namespace stonks::finance {

namespace {
struct AssetTableRow {
  int id{};
  std::string name{};
};

std::optional<std::vector<AssetTableRow>> SelectAssetTable(db::Db &db) {
  const auto rows =
      db.Select("SELECT * FROM Asset;", kAssetTableDefinition.columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get assets from DB");
    return std::nullopt;
  }

  auto result = std::vector<AssetTableRow>{};
  result.reserve(rows->size());

  for (const auto &row : *rows) {
    const auto id = row.GetCellValueInt("id");
    const auto name = row.GetCellValueString("name");

    if (!id.has_value() || (name == nullptr)) {
      spdlog::error("Select result doesn't have expected cell");
      return std::nullopt;
    }

    result.emplace_back(AssetTableRow{.id = *id, .name = *name});
  }

  return result;
}

std::optional<std::vector<Symbol>> SelectSymbols(db::Db &db) {
  const auto columns = std::vector<db::Column>{
      db::Column{.name = "base_asset", .data_type = db::DataType::kText},
      db::Column{.name = "quote_asset", .data_type = db::DataType::kText}};
  const auto rows = db.Select(
      "SELECT BaseAsset.name AS base_asset, QuoteAsset.name AS quote_asset "
      "FROM Symbol "
      "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
      "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id;",
      columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get symbols from DB");
    return std::nullopt;
  }

  auto result = std::vector<Symbol>{};
  result.reserve(rows->size());

  for (const auto &row : *rows) {
    const auto base_asset = row.GetCellValueString("base_asset");
    const auto quote_asset = row.GetCellValueString("quote_asset");

    if ((base_asset == nullptr) || (quote_asset == nullptr)) {
      spdlog::error("Select result doesn't have expected cell");
      return std::nullopt;
    }

    result.emplace_back(
        Symbol{.base_asset = *base_asset, .quote_asset = *quote_asset});
  }

  return result;
}

void UpdateAssetTable(db::Db &db, const std::vector<Symbol> &new_symbols) {
  auto asset_table_rows = SelectAssetTable(db);

  if (!asset_table_rows.has_value()) {
    db.CreateTable(kAssetTableDefinition);
    asset_table_rows = SelectAssetTable(db);

    if (!asset_table_rows.has_value()) {
      spdlog::error("Cannot select Asset table");
      return;
    }
  }

  const auto get_asset = [](const AssetTableRow &row) { return row.name; };
  const auto old_assets =
      *asset_table_rows | ranges::views::transform(get_asset) |
      ranges::to_vector | ranges::actions::sort | ranges::actions::unique;

  const auto get_base_asset = [](const Symbol &symbol) {
    return symbol.base_asset;
  };
  const auto get_quote_asset = [](const Symbol &symbol) {
    return symbol.quote_asset;
  };
  const auto new_assets =
      ranges::views::concat(
          new_symbols | ranges::views::transform(get_base_asset),
          new_symbols | ranges::views::transform(get_quote_asset)) |
      ranges::to_vector | ranges::actions::sort | ranges::actions::unique;

  const auto removed_assets =
      ranges::views::set_difference(old_assets, new_assets);
  const auto added_assets =
      ranges::views::set_difference(new_assets, old_assets);

  for (const auto &removed_asset : removed_assets) {
    // TODO: add removing and updating of related tables via trigger
  }

  for (const auto &added_asset : added_assets) {
    const auto success = db.Insert(
        kAssetTableDefinition.table,
        stonks::db::Row{.cells = {stonks::db::Cell{.column_name = "name",
                                                   .value = added_asset}}});

    if (!success) {
      spdlog::error("Cannot insert asset");
      return;
    }
  }
}

void UpdateSymbolTable(db::Db &db, std::vector<Symbol> new_symbols) {
  const auto asset_table_rows = SelectAssetTable(db);

  if (!asset_table_rows.has_value()) {
    spdlog::error("Cannot select Asset table");
    return;
  }

  auto old_symbols = SelectSymbols(db);

  if (!old_symbols.has_value()) {
    db.CreateTable(kSymbolTableDefinition);
    old_symbols = SelectSymbols(db);

    if (!old_symbols.has_value()) {
      spdlog::error("Cannot select Symbol table");
      return;
    }
  }

  *old_symbols |= ranges::actions::sort | ranges::actions::unique;
  new_symbols |= ranges::actions::sort | ranges::actions::unique;

  const auto removed_symbols =
      ranges::views::set_difference(*old_symbols, new_symbols);
  const auto added_symbols =
      ranges::views::set_difference(new_symbols, *old_symbols);

  for (const auto &removed_symbol : removed_symbols) {
    // TODO: add removing and updating of related tables via trigger
  }

  for (const auto &added_symbol : added_symbols) {
    const auto base_asset = ranges::find_if(
        *asset_table_rows, [&added_symbol](const AssetTableRow &asset) {
          return asset.name == added_symbol.base_asset;
        });
    assert(base_asset != asset_table_rows->end());

    const auto quote_asset = ranges::find_if(
        *asset_table_rows, [&added_symbol](const AssetTableRow &asset) {
          return asset.name == added_symbol.quote_asset;
        });
    assert(quote_asset != asset_table_rows->end());

    const auto success = db.Insert(
        kSymbolTableDefinition.table,
        stonks::db::Row{
            .cells = {stonks::db::Cell{.column_name = "base_asset_id",
                                       .value = base_asset->id},
                      stonks::db::Cell{.column_name = "quote_asset_id",
                                       .value = quote_asset->id}}});

    if (!success) {
      spdlog::error("Cannot insert symbol");
      return;
    }
  }
}

void UpdateTables(db::Db &db) {
  const auto symbols = GetAllSymbols();

  if (!symbols.has_value()) {
    spdlog::error("Cannot get all symbols to update DB");
    return;
  }

  UpdateAssetTable(db, *symbols);
  UpdateSymbolTable(db, *symbols);
}
}  // namespace

class FinanceDb::Impl {
 public:
  explicit Impl(std::string_view uri)
      : sqlite_db_{db::sqlite::SqliteDb::OpenOrCreateDbFromFile(uri)} {}

  db::Db &GetDb() {
    if (!sqlite_db_.has_value()) {
      return db::NullDb::Instance();
    }

    return *sqlite_db_;
  }

 private:
  std::optional<db::sqlite::SqliteDb> sqlite_db_{};
};

FinanceDb::FinanceDb(std::string_view uri)
    : impl_{std::make_unique<Impl>(uri)} {
  UpdateTables(impl_->GetDb());
}

FinanceDb::~FinanceDb() = default;

std::optional<std::vector<std::string>> FinanceDb::SelectAssets() const {
  const auto rows = impl_->GetDb().Select("SELECT name FROM Asset;",
                                          kAssetTableDefinition.columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get assets from DB");
    return std::nullopt;
  }

  auto result = std::vector<std::string>{};
  result.reserve(rows->size());

  for (const auto &row : *rows) {
    const auto asset = row.GetCellValueString("name");

    if (asset == nullptr) {
      spdlog::error("Select result doesn't have expected cell");
      return std::nullopt;
    }

    result.emplace_back(*asset);
  }

  return result;
}

std::optional<std::vector<Symbol>> FinanceDb::SelectSymbols() const {
  return ::stonks::finance::SelectSymbols(impl_->GetDb());
}
}  // namespace stonks::finance