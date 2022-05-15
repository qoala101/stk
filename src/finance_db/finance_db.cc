#include "finance_db.h"

#include <absl/base/macros.h>
#include <spdlog/spdlog.h>

#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/to_container.hpp>
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
struct AssetRecord {
  int64_t id{};
  std::string name{};
} __attribute__((aligned(64)));  // NOLINT(*-magic-numbers)

struct SymbolRecord {
  int64_t id{};
  int64_t base_asset_id{};
  int64_t quote_asset_id{};
  std::string base_asset_name{};
  std::string quote_asset_name{};
} __attribute__((aligned(128)));  // NOLINT(*-magic-numbers)
}  // namespace

struct FinanceDb::Impl {
  explicit Impl(std::string_view uri)
      : sqlite_db{db::sqlite::SqliteDb::OpenOrCreateDbFromFile(uri)} {}

  auto GetDb() -> db::Db & {
    if (!sqlite_db.has_value()) {
      return db::NullDb::Instance();
    }

    return *sqlite_db;
  }

  auto SelectAssets() -> std::optional<std::vector<std::string>> {
    const auto rows =
        GetDb().Select("SELECT * FROM Asset", kAssetTableDefinition.columns);

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

  auto CreateAssetTable() -> bool {
    return GetDb().CreateTable(kAssetTableDefinition);
  }

  auto DeleteAssetFromTable(std::string_view asset) -> bool {
    return GetDb().Delete(kAssetTableDefinition.table,
                          "Asset.name = " + std::string{asset});
  }

  auto InsertAssetIntoTable(std::string_view asset) -> bool {
    return GetDb().Insert(kAssetTableDefinition.table,
                          stonks::db::Row{.cells = {stonks::db::Cell{
                                              .column_name = "name",
                                              .value = {std::string{asset}}}}});
  }

  auto UpdateAssetTable(const std::vector<Symbol> &new_symbols) -> bool {
    auto old_assets = SelectAssets();

    if (!old_assets.has_value()) {
      spdlog::error("Cannot get existing assets");
      return false;
    }

    *old_assets |= ranges::actions::sort | ranges::actions::unique;

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

    auto &db = GetDb();

    const auto removed_assets =
        ranges::views::set_difference(*old_assets, new_assets);

    for (const auto &removed_asset : removed_assets) {
      if (const auto failure = !DeleteAssetFromTable(removed_asset)) {
        spdlog::error("Cannot delete asset");
        return false;
      }
    }

    const auto added_assets =
        ranges::views::set_difference(new_assets, *old_assets);

    for (const auto &added_asset : added_assets) {  // NOLINT(*-use-anyofallof)
      if (const auto failure = !InsertAssetIntoTable(added_asset)) {
        spdlog::error("Cannot insert asset");
        return false;
      }
    }

    return true;
  }

  auto UpdateAssetRecords() -> bool {
    const auto rows =
        GetDb().Select("SELECT * FROM Asset", kAssetTableDefinition.columns);

    if (!rows.has_value()) {
      spdlog::error("Cannot get assets from DB");
      return false;
    }

    asset_records.reserve(rows->size());

    for (const auto &row : *rows) {  // NOLINT(*-use-anyofallof)
      const auto id = row.GetCellValueInt64("id");
      const auto *const name = row.GetCellValueString("name");

      if (!id.has_value() || (name == nullptr)) {
        spdlog::error("Select result doesn't have expected cell");
        return false;
      }

      asset_records.emplace_back(AssetRecord{.id = *id, .name = *name});
    }

    return true;
  }

  auto SelectSymbols() -> std::optional<std::vector<Symbol>> {
    const auto columns = std::vector<db::Column>{
        db::Column{.name = "base_asset", .data_type = db::DataType::kText},
        db::Column{.name = "quote_asset", .data_type = db::DataType::kText}};
    const auto rows = GetDb().Select(
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

  auto CreateSymbolTable() -> bool {
    return GetDb().CreateTable(kSymbolTableDefinition);
  }

  auto DeleteSymbolFromTable(const Symbol &symbol) -> bool {
    return GetDb().Delete(
        kSymbolTableDefinition.table,
        "WHERE Symbol.id = ("
        "SELECT Symbol.id "
        "FROM Symbol "
        "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
        "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id "
        "WHERE BaseAsset.name = \"" +
            symbol.base_asset + "\" AND QuoteAsset.name = \"" +
            symbol.quote_asset + "\")");
  }

  auto InsertSymbolIntoTable(const Symbol &symbol) -> bool {
    const auto base_asset = ranges::find_if(
        asset_records, [&symbol](const AssetRecord &asset_record) {
          return asset_record.name == symbol.base_asset;
        });
    ABSL_ASSERT(base_asset != asset_records.end());

    const auto quote_asset = ranges::find_if(
        asset_records, [&symbol](const AssetRecord &asset_record) {
          return asset_record.name == symbol.quote_asset;
        });
    ABSL_ASSERT(quote_asset != asset_records.end());

    return GetDb().Insert(
        kSymbolTableDefinition.table,
        stonks::db::Row{
            .cells = {stonks::db::Cell{.column_name = "base_asset_id",
                                       .value = {base_asset->id}},
                      stonks::db::Cell{.column_name = "quote_asset_id",
                                       .value = {quote_asset->id}}}});
  }

  auto UpdateSymbolTable(std::vector<Symbol> new_symbols) -> bool {
    auto old_symbols = SelectSymbols();

    if (!old_symbols.has_value()) {
      spdlog::error("Cannot get existing symbols");
      return false;
    }

    *old_symbols |= ranges::actions::sort | ranges::actions::unique;
    new_symbols |= ranges::actions::sort | ranges::actions::unique;

    auto &db = GetDb();
    const auto removed_symbols =
        ranges::views::set_difference(*old_symbols, new_symbols);

    for (const auto &removed_symbol : removed_symbols) {
      if (const auto failure = !DeleteSymbolFromTable(removed_symbol)) {
        spdlog::error("Cannot delete asset");
        return false;
      }
    }

    const auto added_symbols =
        ranges::views::set_difference(new_symbols, *old_symbols);

    // NOLINTNEXTLINE(*-use-anyofallof)
    for (const auto &added_symbol : added_symbols) {
      if (const auto failure = !InsertSymbolIntoTable(added_symbol)) {
        spdlog::error("Cannot insert symbol");
        return false;
      }
    }

    return true;
  }

  auto UpdateSymbolRecords() -> bool {
    const auto columns = std::vector<db::Column>{
        db::Column{.name = "id", .data_type = db::DataType::kInteger},
        db::Column{.name = "base_asset_id",
                   .data_type = db::DataType::kInteger},
        db::Column{.name = "quote_asset_id",
                   .data_type = db::DataType::kInteger},
        db::Column{.name = "base_asset", .data_type = db::DataType::kText},
        db::Column{.name = "quote_asset", .data_type = db::DataType::kText}};
    const auto rows = GetDb().Select(
        "SELECT Symbol.id, Symbol.base_asset_id, Symbol.quote_asset_id, "
        "BaseAsset.name AS base_asset, QuoteAsset.name AS quote_asset "
        "FROM Symbol "
        "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
        "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id",
        columns);

    if (!rows.has_value()) {
      spdlog::error("Cannot get symbols from DB");
      return false;
    }

    symbol_records.reserve(rows->size());

    for (const auto &row : *rows) {  // NOLINT(*-use-anyofallof)
      const auto id = row.GetCellValueInt64("id");
      const auto base_asset_id = row.GetCellValueInt64("base_asset_id");
      const auto quote_asset_id = row.GetCellValueInt64("quote_asset_id");
      const auto *const base_asset = row.GetCellValueString("base_asset");
      const auto *const quote_asset = row.GetCellValueString("quote_asset");

      if (!id.has_value() || !base_asset_id.has_value() ||
          !quote_asset_id.has_value() || (base_asset == nullptr) ||
          (quote_asset == nullptr)) {
        spdlog::error("Select result doesn't have expected cell");
        return false;
      }

      symbol_records.emplace_back(
          SymbolRecord{.id = *id,
                       .base_asset_id = *base_asset_id,
                       .quote_asset_id = *quote_asset_id,
                       .base_asset_name = *base_asset,
                       .quote_asset_name = *quote_asset});
    }

    return true;
  }

  auto GetSymbolIdBySymbol(const Symbol &symbol) -> std::optional<int64_t> {
    const auto symbol_record =
        ranges::find_if(symbol_records, [&symbol](const SymbolRecord &record) {
          return (record.base_asset_name == symbol.base_asset) &&
                 (record.quote_asset_name == symbol.quote_asset);
        });

    if (symbol_record == symbol_records.end()) {
      spdlog::error("Cannot find symbol record");
      return std::nullopt;
    }

    return symbol_record->id;
  }

  auto GetSymbolById(int64_t symbol_id) -> std::optional<Symbol> {
    const auto symbol_record = ranges::find_if(
        symbol_records, [symbol_id](const SymbolRecord &record) {
          return record.id == symbol_id;
        });

    if (symbol_record == symbol_records.end()) {
      spdlog::error("Cannot find symbol record");
      return std::nullopt;
    }

    return Symbol{.base_asset = symbol_record->base_asset_name,
                  .quote_asset = symbol_record->quote_asset_name};
  }

  auto GetSymbolByName(std::string_view symbol_name) -> std::optional<Symbol> {
    const auto symbol_record = ranges::find_if(
        symbol_records, [symbol_name](const SymbolRecord &record) {
          return (record.base_asset_name + record.quote_asset_name) ==
                 symbol_name;
        });

    if (symbol_record == symbol_records.end()) {
      spdlog::error("Cannot find symbol record");
      return std::nullopt;
    }

    return Symbol{.base_asset = symbol_record->base_asset_name,
                  .quote_asset = symbol_record->quote_asset_name};
  }

  auto SelectSymbolPriceTicks(
      std::optional<int> limit = std::nullopt,
      const std::optional<Period> &period = std::nullopt,
      const std::optional<std::vector<Symbol>> &symbols = std::nullopt)
      -> std::optional<std::vector<SymbolPriceTick>> {
    auto query = std::string{"SELECT * FROM SymbolPriceTick"};
    auto condition = std::string{};

    auto add_and = false;

    if (period.has_value()) {
      if (period->start_time.has_value()) {
        if (add_and) {
          condition += " AND ";
        }

        condition += "SymbolPriceTick.time >= " +
                     std::to_string(period->start_time->count());

        add_and = true;
      }

      if (period->end_time.has_value()) {
        if (add_and) {
          condition += " AND ";
        }

        condition += "SymbolPriceTick.time <= " +
                     std::to_string(period->end_time->count());

        add_and = true;
      }
    }

    if (symbols.has_value()) {
      auto symbol_ids = std::string{};

      for (const auto &symbol : *symbols) {
        const auto symbol_id = GetSymbolIdBySymbol(symbol);

        if (!symbol_id.has_value()) {
          spdlog::error("Symbol not found in DB");
          return std::nullopt;
        }

        symbol_ids += std::to_string(*symbol_id);

        if (const auto not_last_item = &symbol != &symbols->back()) {
          symbol_ids += ", ";
        }
      }

      if (!symbol_ids.empty()) {
        if (add_and) {
          condition += " AND ";
        }

        condition += "SymbolPriceTick.symbol_id IN (" + symbol_ids + ")";

        add_and = true;
      }
    }

    if (!condition.empty()) {
      query += " WHERE " + condition;
    }

    if (limit.has_value()) {
      query += " LIMIT " + std::to_string(*limit);
    }

    const auto rows =
        GetDb().Select(query, kSymbolPriceTickTableDefinition.columns);

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

      if (!symbol_id.has_value() || !time.has_value() ||
          !buy_price.has_value() || !sell_price.has_value()) {
        spdlog::error("Select result doesn't have expected cell");
        return std::nullopt;
      }

      const auto symbol = GetSymbolById(*symbol_id);

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

  auto CreateSymbolPriceTickTable() -> bool {
    return GetDb().CreateTable(kSymbolPriceTickTableDefinition);
  }

  auto InsertSymbolPriceTick(const SymbolPriceTick &symbol_price_tick) -> bool {
    const auto symbol_id = GetSymbolIdBySymbol(symbol_price_tick.symbol);

    if (!symbol_id.has_value()) {
      spdlog::error("Symbol not found in DB");
      return false;
    }

    const auto success = GetDb().Insert(
        kSymbolPriceTickTableDefinition.table,
        db::Row{.cells = {
                    db::Cell{.column_name = "symbol_id", .value = {*symbol_id}},
                    db::Cell{.column_name = "time",
                             .value = {symbol_price_tick.time.count()}},
                    db::Cell{.column_name = "buy_price",
                             .value = {symbol_price_tick.buy_price}},
                    db::Cell{.column_name = "sell_price",
                             .value = {symbol_price_tick.sell_price}}}});

    if (!success) {
      spdlog::error("Cannot insert symbol price tick");
      return false;
    }

    return true;
  }

  // NOLINTNEXTLINE(*-non-private-member-variables-in-classes)
  std::optional<db::sqlite::SqliteDb> sqlite_db{};
  // NOLINTNEXTLINE(*-non-private-member-variables-in-classes)
  std::vector<AssetRecord> asset_records{};
  // NOLINTNEXTLINE(*-non-private-member-variables-in-classes)
  std::vector<SymbolRecord> symbol_records{};
} __attribute__((aligned(128)));  // NOLINT(*-magic-numbers)

FinanceDb::FinanceDb(std::string_view uri)
    : impl_{std::make_unique<Impl>(uri)} {
  if (!impl_->CreateAssetTable()) {
    spdlog::error("Cannot create Asset table");
    return;
  }

  if (!impl_->CreateSymbolTable()) {
    spdlog::error("Cannot create Asset table");
    return;
  }

  if (!impl_->CreateSymbolPriceTickTable()) {
    spdlog::error("Cannot create SymbolPrice table");
    return;
  }

  auto symbols = GetAllSymbols();

  if (!symbols.has_value()) {
    spdlog::error("Cannot get all symbols to update DB");
    return;
  }

  if (!impl_->UpdateAssetTable(*symbols)) {
    spdlog::error("Cannot update Asset table");
    return;
  }

  if (!impl_->UpdateAssetRecords()) {
    spdlog::error("Cannot update asset records");
    return;
  }

  if (!impl_->UpdateSymbolTable(std::move(*symbols))) {
    spdlog::error("Cannot update Symbol table");
    return;
  }

  if (!impl_->UpdateSymbolRecords()) {
    spdlog::error("Cannot update symbol records");
    return;
  }
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
  return impl_->SelectSymbols();
}

auto FinanceDb::InsertSymbolPriceTick(const SymbolPriceTick &symbol_price_tick)
    -> bool {
  return impl_->InsertSymbolPriceTick(symbol_price_tick);
}

auto FinanceDb::SelectSymbolPriceTicks(
    std::optional<int> limit, const std::optional<Period> &period,
    const std::optional<std::vector<Symbol>> &symbols) const
    -> std::optional<std::vector<SymbolPriceTick>> {
  return impl_->SelectSymbolPriceTicks(limit, period, symbols);
}

auto FinanceDb::SelectSymbolById(int64_t symbol_id) const
    -> std::optional<Symbol> {
  return impl_->GetSymbolById(symbol_id);
}

auto FinanceDb::SelectSymbolByName(std::string_view symbol_name) const
    -> std::optional<Symbol> {
  return impl_->GetSymbolByName(symbol_name);
}

auto FinanceDb::SelectSymbolIdBySymbol(const Symbol &symbol) const
    -> std::optional<int64_t> {
  return impl_->GetSymbolIdBySymbol(symbol);
}
}  // namespace stonks::finance