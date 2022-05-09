#include "finance_db.h"

#include <spdlog/spdlog.h>

#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/transform.hpp>
#include <string>

#include "db_types.h"
#include "finance_api.h"
#include "finance_db_table_definitions.h"
#include "finance_types.h"
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

std::optional<Symbol> SelectSymbolById(db::Db &db, int64_t symbol_id) {
  const auto columns = std::vector<db::Column>{
      db::Column{.name = "base_asset", .data_type = db::DataType::kText},
      db::Column{.name = "quote_asset", .data_type = db::DataType::kText}};
  const auto rows = db.Select(
      "SELECT BaseAsset.name AS base_asset, QuoteAsset.name AS quote_asset "
      "FROM Symbol "
      "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
      "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id "
      "WHERE Symbol.id = " +
          std::to_string(symbol_id) + ";",
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
  const auto base_asset = row.GetCellValueString("base_asset");
  const auto quote_asset = row.GetCellValueString("quote_asset");

  if ((base_asset == nullptr) || (quote_asset == nullptr)) {
    spdlog::error("Select result doesn't have expected cell");
    return std::nullopt;
  }

  return Symbol{.base_asset = *base_asset, .quote_asset = *quote_asset};
}

std::optional<Symbol> SelectSymbolByName(db::Db &db,
                                         std::string_view symbol_name) {
  const auto columns = std::vector<db::Column>{
      db::Column{.name = "base_asset", .data_type = db::DataType::kText},
      db::Column{.name = "quote_asset", .data_type = db::DataType::kText}};
  const auto rows = db.Select(
      "SELECT BaseAsset.name AS base_asset, QuoteAsset.name AS quote_asset "
      "FROM Symbol "
      "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
      "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id "
      "WHERE base_asset || quote_asset = \"" +
          std::string{symbol_name} + "\";",
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
  const auto base_asset = row.GetCellValueString("base_asset");
  const auto quote_asset = row.GetCellValueString("quote_asset");

  if ((base_asset == nullptr) || (quote_asset == nullptr)) {
    spdlog::error("Select result doesn't have expected cell");
    return std::nullopt;
  }

  return Symbol{.base_asset = *base_asset, .quote_asset = *quote_asset};
}

std::optional<int64_t> SelectSymbolIdBySymbol(db::Db &db,
                                              const Symbol &symbol) {
  const auto columns = std::vector<db::Column>{
      db::Column{.name = "id", .data_type = db::DataType::kInteger}};
  const auto rows = db.Select(
      "SELECT Symbol.id "
      "FROM Symbol "
      "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
      "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id "
      "WHERE BaseAsset.name || QuoteAsset.name = \"" +
          symbol.base_asset + symbol.quote_asset + "\";",
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

std::optional<std::vector<SymbolBookTick>> SelectSymbolBookTicks(
    db::Db &db, const std::optional<Symbol> &symbol = std::nullopt) {
  auto query = std::string{"SELECT * FROM SymbolBookTick"};

  if (symbol.has_value()) {
    const auto symbol_id = SelectSymbolIdBySymbol(db, *symbol);

    if (!symbol_id.has_value()) {
      spdlog::error("Symbol not found in DB");
      return std::nullopt;
    }

    query += " WHERE SymbolBookTick.symbol_id = " + std::to_string(*symbol_id);
  }

  query += ";";

  const auto rows = db.Select(query, kSymbolBookTickTableDefinition.columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get symbol book ticks from DB");
    return std::nullopt;
  }

  auto result = std::vector<SymbolBookTick>{};
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

    result.emplace_back(SymbolBookTick{.symbol = *symbol,
                                       .time = std::chrono::milliseconds{*time},
                                       .buy_price = *buy_price,
                                       .sell_price = *sell_price});
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
                                                   .value = {added_asset}}}});

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
                                       .value = {base_asset->id}},
                      stonks::db::Cell{.column_name = "quote_asset_id",
                                       .value = {quote_asset->id}}}});

    if (!success) {
      spdlog::error("Cannot insert symbol");
      return;
    }
  }
}

void UpdateSymbolBookTickTable(db::Db &db) {
  auto symbol_book_ticks = SelectSymbolBookTicks(db);

  if (!symbol_book_ticks.has_value()) {
    db.CreateTable(kSymbolBookTickTableDefinition);
    symbol_book_ticks = SelectSymbolBookTicks(db);

    if (!symbol_book_ticks.has_value()) {
      spdlog::error("Cannot create Symbol Book Tick table");
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
  UpdateSymbolBookTickTable(db);
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

bool FinanceDb::InsertSymbolsPrices(
    const std::vector<SymbolPrices> &symbols_prices) {
  for (const auto &symbol_prices : symbols_prices) {
    for (const auto &price : symbol_prices.prices) {
      const auto success = impl_->GetDb().Insert(
          kSymbolPriceTableDefinition.table,
          db::Row{.cells = {db::Cell{.column_name = "time",
                                     .value = {price.time.count()}}}});

      if (!success) {
        spdlog::info("Cannot insert symbol price {}",
                     symbol_prices.symbol.GetName());
        return false;
      }
    }
  }

  return true;
}

std::optional<std::vector<TimeDouble>> FinanceDb::SelectSymbolPrices(
    const Symbol &symbol) const {
  const auto columns = std::vector<db::Column>{
      db::Column{.name = "time", .data_type = db::DataType::kInteger},
      db::Column{.name = "price", .data_type = db::DataType::kReal}};
  const auto rows = impl_->GetDb().Select(
      "SELECT time, price FROM SymbolPrice JOIN Symbol ON "
      "SymbolPrice.symbol_id = Symbol.id JOIN Asset AS BaseAsset ON "
      "Symbol.base_asset_id = BaseAsset.id JOIN Asset AS QuoteAsset ON "
      "Symbol.quote_asset_id = QuoteAsset.id WHERE BaseAsset.name = \"" +
          symbol.base_asset + "\" AND QuoteAsset.name = \"" +
          symbol.quote_asset + "\";",
      columns);

  if (!rows.has_value()) {
    spdlog::error("Cannot get symbol prices from DB");
    return std::nullopt;
  }

  auto result = std::vector<TimeDouble>{};
  result.reserve(rows->size());

  for (const auto &row : *rows) {
    const auto time = row.GetCellValueInt64("time");
    const auto price = row.GetCellValueDouble("price");

    if (!time.has_value() || !price.has_value()) {
      spdlog::error("Select result doesn't have expected cell");
      return std::nullopt;
    }

    result.emplace_back(
        TimeDouble{.time = std::chrono::milliseconds{*time}, .value = *price});
  }

  return result;
}

bool FinanceDb::InsertSymbolBookTick(const SymbolBookTick &symbol_book_tick) {
  const auto symbol_id = SelectSymbolIdBySymbol(symbol_book_tick.symbol);

  if (!symbol_id.has_value()) {
    spdlog::error("Symbol not found in DB");
    return false;
  }

  const auto success = impl_->GetDb().Insert(
      kSymbolBookTickTableDefinition.table,
      db::Row{
          .cells = {db::Cell{.column_name = "symbol_id", .value = {*symbol_id}},
                    db::Cell{.column_name = "time",
                             .value = {symbol_book_tick.time.count()}},
                    db::Cell{.column_name = "buy_price",
                             .value = {symbol_book_tick.buy_price}},
                    db::Cell{.column_name = "sell_price",
                             .value = {symbol_book_tick.sell_price}}}});

  if (!success) {
    spdlog::info("Cannot insert symbol book tick {}",
                 symbol_book_tick.symbol.GetName());
    return false;
  }

  return true;
}

std::optional<std::vector<SymbolBookTick>> FinanceDb::SelectSymbolBookTicks(
    const Symbol &symbol) const {
  return ::stonks::finance::SelectSymbolBookTicks(impl_->GetDb(), symbol);
}

std::optional<Symbol> FinanceDb::SelectSymbolById(int64_t symbol_id) const {
  return ::stonks::finance::SelectSymbolById(impl_->GetDb(), symbol_id);
}

std::optional<Symbol> FinanceDb::SelectSymbolByName(
    std::string_view symbol_name) const {
  return ::stonks::finance::SelectSymbolByName(impl_->GetDb(), symbol_name);
}

std::optional<int64_t> FinanceDb::SelectSymbolIdBySymbol(
    const Symbol &symbol) const {
  return ::stonks::finance::SelectSymbolIdBySymbol(impl_->GetDb(), symbol);
}
}  // namespace stonks::finance