#include "finance_db.h"

#include <absl/base/macros.h>
#include <spdlog/spdlog.h>

#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/adaptor.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/transform.hpp>

#include "db_types.h"
#include "finance_api.h"
#include "finance_db_table_definitions.h"
#include "finance_types.h"
#include "null_db.h"
#include "sqlite_db.h"

namespace stonks::finance {
namespace {
auto SelectSymbolById(db::Db &db, int64_t symbol_id) -> std::optional<Symbol> {
  const auto columns = std::vector<db::Column>{
      db::Column{.name = "base_asset", .data_type = db::DataType::kText},
      db::Column{.name = "quote_asset", .data_type = db::DataType::kText}};
  const auto rows = db.Select(
      "SELECT BaseAsset.name AS base_asset, QuoteAsset.name AS quote_asset "
      "FROM Symbol "
      "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
      "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id "
      "WHERE Symbol.id = " +
          std::to_string(symbol_id),
      columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get symbol from DB");
    return std::nullopt;
  }

  if (rows->empty()) {
    spdlog::error("No symbol with ID {}", symbol_id);
    return std::nullopt;
  }

  const auto &row = (*rows)[0];
  const auto *const base_asset = row.GetCellValueString("base_asset");
  const auto *const quote_asset = row.GetCellValueString("quote_asset");

  if ((base_asset == nullptr) || (quote_asset == nullptr)) {
    spdlog::error("Select result doesn't have expected cell");
    return std::nullopt;
  }

  return Symbol{.base_asset = *base_asset, .quote_asset = *quote_asset};
}

auto SelectSymbolByName(db::Db &db, std::string_view symbol_name)
    -> std::optional<Symbol> {
  const auto columns = std::vector<db::Column>{
      db::Column{.name = "base_asset", .data_type = db::DataType::kText},
      db::Column{.name = "quote_asset", .data_type = db::DataType::kText}};
  const auto rows = db.Select(
      "SELECT BaseAsset.name AS base_asset, QuoteAsset.name AS quote_asset "
      "FROM Symbol "
      "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
      "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id "
      "WHERE base_asset || quote_asset = \"" +
          std::string{symbol_name} + "\"",
      columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get symbol from DB");
    return std::nullopt;
  }

  if (rows->empty()) {
    spdlog::error("No symbol with name {}", symbol_name);
    return std::nullopt;
  }

  const auto &row = (*rows)[0];
  const auto *const base_asset = row.GetCellValueString("base_asset");
  const auto *const quote_asset = row.GetCellValueString("quote_asset");

  if ((base_asset == nullptr) || (quote_asset == nullptr)) {
    spdlog::error("Select result doesn't have expected cell");
    return std::nullopt;
  }

  return Symbol{.base_asset = *base_asset, .quote_asset = *quote_asset};
}

auto SelectSymbolIdBySymbol(db::Db &db, const Symbol &symbol)
    -> std::optional<int64_t> {
  const auto columns = std::vector<db::Column>{
      db::Column{.name = "id", .data_type = db::DataType::kInteger}};
  const auto rows = db.Select(
      "SELECT Symbol.id "
      "FROM Symbol "
      "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
      "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id "
      "WHERE BaseAsset.name || QuoteAsset.name = \"" +
          symbol.base_asset + symbol.quote_asset + "\"",
      columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get symbol from DB");
    return std::nullopt;
  }

  if (rows->empty()) {
    spdlog::error("No symbol with assets {} {}", symbol.base_asset,
                  symbol.quote_asset);
    return std::nullopt;
  }

  const auto &row = (*rows)[0];
  const auto id = row.GetCellValueInt64("id");

  if (!id.has_value()) {
    spdlog::error("Select result doesn't have expected cell");
    return std::nullopt;
  }

  return *id;
}

auto SelectAssetIdByAsset(db::Db &db, std::string_view asset)
    -> std::optional<int64_t> {
  const auto columns = std::vector<db::Column>{
      db::Column{.name = "id", .data_type = db::DataType::kInteger}};
  const auto rows =
      db.Select("SELECT Asset.id FROM Asset WHERE Asset.name = \"" +
                    std::string{asset} + "\"",
                columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get asset from DB");
    return std::nullopt;
  }

  if (rows->empty()) {
    spdlog::error("No asset with name {}", asset);
    return std::nullopt;
  }

  const auto &row = (*rows)[0];
  const auto id = row.GetCellValueInt64("id");

  if (!id.has_value()) {
    spdlog::error("Select result doesn't have expected cell");
    return std::nullopt;
  }

  return *id;
}

auto SelectAssets(db::Db &db) -> std::optional<std::vector<std::string>> {
  const auto rows =
      db.Select("SELECT * FROM Asset", kAssetTableDefinition.columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get assets from DB");
    return std::nullopt;
  }

  auto result = std::vector<std::string>{};
  result.reserve(rows->size());

  for (const auto &row : *rows) {
    const auto *const name = row.GetCellValueString("name");

    if (name == nullptr) {
      spdlog::error("Select result doesn't have expected cell");
      return std::nullopt;
    }

    result.emplace_back(*name);
  }

  return result;
}

auto SelectSymbols(db::Db &db) -> std::optional<std::vector<Symbol>> {
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
    const auto *const base_asset = row.GetCellValueString("base_asset");
    const auto *const quote_asset = row.GetCellValueString("quote_asset");

    if ((base_asset == nullptr) || (quote_asset == nullptr)) {
      spdlog::error("Select result doesn't have expected cell");
      return std::nullopt;
    }

    result.emplace_back(
        Symbol{.base_asset = *base_asset, .quote_asset = *quote_asset});
  }

  return result;
}

auto SelectSymbolPriceTicks(db::Db &db,
                            const std::optional<Symbol> &symbol = std::nullopt)
    -> std::optional<std::vector<SymbolPriceTick>> {
  auto query = std::string{"SELECT * FROM SymbolPriceTick"};

  if (symbol.has_value()) {
    const auto symbol_id = SelectSymbolIdBySymbol(db, *symbol);

    if (!symbol_id.has_value()) {
      spdlog::error("Symbol not found in DB");
      return std::nullopt;
    }

    query += " WHERE SymbolPriceTick.symbol_id = " + std::to_string(*symbol_id);
  }

  const auto rows = db.Select(query, kSymbolPriceTickTableDefinition.columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get symbol price ticks from DB");
    return std::nullopt;
  }

  auto result = std::vector<SymbolPriceTick>{};
  result.reserve(rows->size());

  for (const auto &row : *rows) {
    const auto symbol_id = row.GetCellValueInt64("symbol_id");
    const auto time = row.GetCellValueInt64("time");
    const auto buy_price = row.GetCellValueDouble("buy_price");
    const auto sell_price = row.GetCellValueDouble("sell_price");

    if (!symbol_id.has_value() || !time.has_value() || !buy_price.has_value() ||
        !sell_price.has_value()) {
      spdlog::error("Select result doesn't have expected cell");
      return std::nullopt;
    }

    const auto symbol = SelectSymbolById(db, *symbol_id);

    if (!symbol.has_value()) {
      spdlog::error("No symbol with ID {}", *symbol_id);
      return std::nullopt;
    }

    result.emplace_back(
        SymbolPriceTick{.symbol = *symbol,
                        .time = std::chrono::milliseconds{*time},
                        .buy_price = *buy_price,
                        .sell_price = *sell_price});
  }

  return result;
}

void UpdateAssetTable(db::Db &db, const std::vector<Symbol> &new_symbols) {
  auto assets = SelectAssets(db);

  if (!assets.has_value()) {
    db.CreateTable(kAssetTableDefinition);
    assets = SelectAssets(db);

    if (!assets.has_value()) {
      spdlog::error("Cannot select Asset table");
      return;
    }
  }

  auto &old_assets = *assets;
  old_assets |= ranges::actions::sort | ranges::actions::unique;

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
    // TODO(vh): add removing and updating of related tables via trigger
  }

  for (const auto &added_asset : added_assets) {
    const auto success = db.Insert(
        kAssetTableDefinition.table,
        stonks::db::Row{.cells = {stonks::db::Cell{.column_name = "name",
                                                   .value = {added_asset}}}});

    if (!success) {
      spdlog::error("Cannot insert asset");
      return;
    }
  }
}

auto InsertSymbolIntoSymbolTable(db::Db &db,
                                 const std::vector<std::string> &assets,
                                 const Symbol &symbol) -> bool {
  const auto base_asset =
      ranges::find_if(assets, [&symbol](const std::string &asset) {
        return asset == symbol.base_asset;
      });
  ABSL_ASSERT(base_asset != assets.end());
  const auto base_asset_id = SelectAssetIdByAsset(db, *base_asset);

  if (!base_asset_id.has_value()) {
    spdlog::error("Cannot find asset in DB");
    return false;
  }

  const auto quote_asset =
      ranges::find_if(assets, [&symbol](const std::string &asset) {
        return asset == symbol.quote_asset;
      });
  ABSL_ASSERT(quote_asset != assets.end());
  const auto quote_asset_id = SelectAssetIdByAsset(db, *quote_asset);

  if (!quote_asset_id.has_value()) {
    spdlog::error("Cannot find asset in DB");
    return false;
  }

  return db.Insert(
      kSymbolTableDefinition.table,
      stonks::db::Row{
          .cells = {stonks::db::Cell{.column_name = "base_asset_id",
                                     .value = {*base_asset_id}},
                    stonks::db::Cell{.column_name = "quote_asset_id",
                                     .value = {*quote_asset_id}}}});
}

void UpdateSymbolTable(db::Db &db, std::vector<Symbol> new_symbols) {
  const auto assets = SelectAssets(db);

  if (!assets.has_value()) {
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
    // TODO(vh): add removing and updating of related tables via trigger
  }

  for (const auto &added_symbol : added_symbols) {
    if (!InsertSymbolIntoSymbolTable(db, *assets, added_symbol)) {
      spdlog::error("Cannot insert symbol");
      return;
    }
  }
}

void UpdateSymbolPriceTickTable(db::Db &db) {
  auto symbol_price_ticks = SelectSymbolPriceTicks(db);

  if (!symbol_price_ticks.has_value()) {
    db.CreateTable(kSymbolPriceTickTableDefinition);
    symbol_price_ticks = SelectSymbolPriceTicks(db);

    if (!symbol_price_ticks.has_value()) {
      spdlog::error("Cannot create Symbol Price Tick table");
    }
  }
}
}  // namespace

class FinanceDb::Impl {
 public:
  explicit Impl(std::string_view uri)
      : sqlite_db_{db::sqlite::SqliteDb::OpenOrCreateDbFromFile(uri)} {}

  auto GetDb() -> db::Db & {
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
  const auto symbols = GetAllSymbols();

  if (!symbols.has_value()) {
    spdlog::error("Cannot get all symbols to update DB");
    return;
  }

  auto &db = impl_->GetDb();
  UpdateAssetTable(db, *symbols);
  UpdateSymbolTable(db, *symbols);
  UpdateSymbolPriceTickTable(db);
}

FinanceDb::~FinanceDb() = default;

auto FinanceDb::SelectAssets() const
    -> std::optional<std::vector<std::string>> {
  const auto rows = impl_->GetDb().Select("SELECT name FROM Asset;",
                                          kAssetTableDefinition.columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get assets from DB");
    return std::nullopt;
  }

  auto result = std::vector<std::string>{};
  result.reserve(rows->size());

  for (const auto &row : *rows) {
    const auto *const asset = row.GetCellValueString("name");

    if (asset == nullptr) {
      spdlog::error("Select result doesn't have expected cell");
      return std::nullopt;
    }

    result.emplace_back(*asset);
  }

  return result;
}

auto FinanceDb::SelectSymbols() const -> std::optional<std::vector<Symbol>> {
  return ::stonks::finance::SelectSymbols(impl_->GetDb());
}

auto FinanceDb::InsertSymbolPriceTick(const SymbolPriceTick &symbol_price_tick)
    -> bool {
  const auto symbol_id = SelectSymbolIdBySymbol(symbol_price_tick.symbol);

  if (!symbol_id.has_value()) {
    spdlog::error("Symbol not found in DB");
    return false;
  }

  const auto success = impl_->GetDb().Insert(
      kSymbolPriceTickTableDefinition.table,
      db::Row{
          .cells = {db::Cell{.column_name = "symbol_id", .value = {*symbol_id}},
                    db::Cell{.column_name = "time",
                             .value = {symbol_price_tick.time.count()}},
                    db::Cell{.column_name = "buy_price",
                             .value = {symbol_price_tick.buy_price}},
                    db::Cell{.column_name = "sell_price",
                             .value = {symbol_price_tick.sell_price}}}});

  if (!success) {
    spdlog::info("Cannot insert symbol book tick {}",
                 symbol_price_tick.symbol.GetName());
    return false;
  }

  return true;
}

auto FinanceDb::SelectSymbolPriceTicks(const std::optional<Symbol> &symbol)
    const -> std::optional<std::vector<SymbolPriceTick>> {
  return ::stonks::finance::SelectSymbolPriceTicks(impl_->GetDb(), symbol);
}

auto FinanceDb::SelectSymbolById(int64_t symbol_id) const
    -> std::optional<Symbol> {
  return ::stonks::finance::SelectSymbolById(impl_->GetDb(), symbol_id);
}

auto FinanceDb::SelectSymbolByName(std::string_view symbol_name) const
    -> std::optional<Symbol> {
  return ::stonks::finance::SelectSymbolByName(impl_->GetDb(), symbol_name);
}

auto FinanceDb::SelectSymbolIdBySymbol(const Symbol &symbol) const
    -> std::optional<int64_t> {
  return ::stonks::finance::SelectSymbolIdBySymbol(impl_->GetDb(), symbol);
}
}  // namespace stonks::finance