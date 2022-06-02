#include "finance_db.h"

#include <absl/base/macros.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <array>
#include <exception>
#include <map>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/algorithm/contains.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>

#include "db_types.h"
#include "finance_db_table_definitions.h"
#include "sqlite_db.h"

namespace stonks::finance {
namespace {
auto Logger() -> spdlog::logger & {
  static auto logger = []() {
    auto logger = spdlog::stdout_color_mt("Db");
    logger->set_level(spdlog::level::debug);
    return logger;
  }();

  return *logger;
}
}  // namespace

class FinanceDb::Impl {
 public:
  explicit Impl(std::string_view uri)
      : sqlite_db_{*db::sqlite::SqliteDb::OpenOrCreateDbFromFile(uri)} {
    sqlite_db_.CreateTableIfNotExists(table_definition::Asset());
    sqlite_db_.CreateTableIfNotExists(table_definition::Symbol());
    sqlite_db_.CreateTableIfNotExists(table_definition::SymbolPriceTick());

    cache_.UpdateFromDb(sqlite_db_);
  }

  [[nodiscard]] auto SelectAssets() -> std::vector<std::string> {
    static constinit const auto kColumnNames = std::array{"name"};

    const auto columns = table_definition::Asset().columns |
                         ranges::views::filter([](const auto &column) {
                           return ranges::contains(kColumnNames, column.name);
                         }) |
                         ranges::to_vector;
    const auto rows = *sqlite_db_.Select("SELECT name FROM Asset", columns);

    return rows | ranges::views::transform([](const auto &row) {
             return *row.GetCellValueString("name");
           }) |
           ranges::to_vector;
  }

  void UpdateAssets(std::vector<std::string> assets) {
    assets |= ranges::actions::sort | ranges::actions::unique;
    const auto old_assets =
        SelectAssets() | ranges::actions::sort | ranges::actions::unique;
    const auto &new_assets = assets;

    const auto removed_assets =
        ranges::views::set_difference(old_assets, new_assets);

    for (const auto &removed_asset : removed_assets) {
      sqlite_db_.Delete(table_definition::Asset().table,
                        "WHERE Asset.name = \"" + removed_asset + "\"");
    }

    const auto added_assets =
        ranges::views::set_difference(new_assets, old_assets);

    for (const auto &added_asset : added_assets) {
      sqlite_db_.Insert(
          table_definition::Asset().table,
          stonks::db::Row{.cells = {stonks::db::Cell{.column_name = "name",
                                                     .value = {added_asset}}}});
    }

    cache_.UpdateFromDb(sqlite_db_);
  }

  [[nodiscard]] auto SelectSymbols() -> std::vector<SymbolName> {
    static constinit const auto kColumnNames = std::array{"name"};

    const auto columns = table_definition::Symbol().columns |
                         ranges::views::filter([](const auto &column) {
                           return ranges::contains(kColumnNames, column.name);
                         }) |
                         ranges::to_vector;
    const auto rows = *sqlite_db_.Select("SELECT name FROM Symbol", {columns});

    return rows | ranges::views::transform([](const auto &row) {
             return SymbolName{*row.GetCellValueString("name")};
           }) |
           ranges::to_vector;
  }

  [[nodiscard]] auto SelectSymbolsInfo() -> std::vector<SymbolInfo> {
    static constinit const auto kColumnNames =
        std::array{"name", "min_base_amount", "min_quote_amount", "base_step",
                   "quote_step"};

    auto columns = table_definition::Symbol().columns |
                   ranges::views::filter([](const auto &column) {
                     return ranges::contains(kColumnNames, column.name);
                   }) |
                   ranges::to_vector;
    columns.emplace_back(
        db::Column{.name = "base_asset", .data_type = db::DataType::kText});
    columns.emplace_back(
        db::Column{.name = "quote_asset", .data_type = db::DataType::kText});
    const auto rows = *sqlite_db_.Select(
        "SELECT Symbol.name, BaseAsset.name AS base_asset, QuoteAsset.name AS "
        "quote_asset, Symbol.min_base_amount, Symbol.min_quote_amount, "
        "Symbol.base_step, Symbol.quote_step "
        "FROM Symbol "
        "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
        "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id",
        columns);

    return rows | ranges::views::transform([](const auto &row) {
             return SymbolInfo{
                 .symbol = *row.GetCellValueString("name"),
                 .base_asset = *row.GetCellValueString("base_asset"),
                 .quote_asset = *row.GetCellValueString("quote_asset"),
                 .min_base_amount = *row.GetCellValueDouble("min_base_amount"),
                 .min_quote_amount =
                     *row.GetCellValueDouble("min_quote_amount"),
                 .base_step = *row.GetCellValueDouble("base_step"),
                 .quote_step = *row.GetCellValueDouble("quote_step")};
           }) |
           ranges::to_vector;
  }

  void InsertSymbolInfo(const SymbolInfo &symbol_info) {
    const auto base_asset_id = cache_.GetAssetIdByAsset(symbol_info.base_asset);

    if (!base_asset_id.has_value()) {
      Logger().error("Asset not found in DB {}", symbol_info.base_asset);
      return;
    }

    const auto quote_asset_id =
        cache_.GetAssetIdByAsset(symbol_info.quote_asset);

    if (!quote_asset_id.has_value()) {
      Logger().error("Asset not found in DB {}", symbol_info.quote_asset);
      return;
    }

    sqlite_db_.Insert(
        table_definition::Symbol().table,
        stonks::db::Row{
            .cells = {stonks::db::Cell{.column_name = "name",
                                       .value = {symbol_info.symbol}},
                      stonks::db::Cell{.column_name = "base_asset_id",
                                       .value = {*base_asset_id}},
                      stonks::db::Cell{.column_name = "quote_asset_id",
                                       .value = {*quote_asset_id}},
                      stonks::db::Cell{.column_name = "min_base_amount",
                                       .value = {symbol_info.min_base_amount}},
                      stonks::db::Cell{.column_name = "min_quote_amount",
                                       .value = {symbol_info.min_quote_amount}},
                      stonks::db::Cell{.column_name = "base_step",
                                       .value = {symbol_info.base_step}},
                      stonks::db::Cell{.column_name = "quote_step",
                                       .value = {symbol_info.quote_step}}}});
  }

  void UpdateSymbolInfoInTable(const SymbolInfo &symbol_info) {
    const auto base_asset_id = cache_.GetAssetIdByAsset(symbol_info.base_asset);

    if (!base_asset_id.has_value()) {
      Logger().error("Asset not found in DB {}", symbol_info.base_asset);
      return;
    }

    const auto quote_asset_id =
        cache_.GetAssetIdByAsset(symbol_info.quote_asset);

    if (!quote_asset_id.has_value()) {
      Logger().error("Asset not found in DB {}", symbol_info.quote_asset);
      return;
    }

    sqlite_db_.Update(
        table_definition::Symbol().table,
        stonks::db::Row{
            .cells = {stonks::db::Cell{.column_name = "base_asset_id",
                                       .value = {*base_asset_id}},
                      stonks::db::Cell{.column_name = "quote_asset_id",
                                       .value = {*quote_asset_id}},
                      stonks::db::Cell{.column_name = "min_base_amount",
                                       .value = {symbol_info.min_base_amount}},
                      stonks::db::Cell{.column_name = "min_quote_amount",
                                       .value = {symbol_info.min_quote_amount}},
                      stonks::db::Cell{.column_name = "base_step",
                                       .value = {symbol_info.base_step}},
                      stonks::db::Cell{.column_name = "quote_step",
                                       .value = {symbol_info.quote_step}}}},
        "WHERE Symbol.name = \"" + symbol_info.symbol + "\"");
  }

  void UpdateSymbolsInfo(std::vector<SymbolInfo> symbols_info) {
    const auto symbol_less = [](const auto &left, const auto &right) {
      return left.symbol < right.symbol;
    };
    const auto symbol_equal = [](const auto &left, const auto &right) {
      return left.symbol == right.symbol;
    };

    symbols_info |= ranges::actions::sort(symbol_less) |
                    ranges::actions::unique(symbol_equal);
    const auto old_symbols_info = SelectSymbolsInfo() |
                                  ranges::actions::sort(symbol_less) |
                                  ranges::actions::unique(symbol_equal);
    const auto &new_symbols_info = symbols_info;

    const auto removed_symbols = ranges::views::set_difference(
        old_symbols_info, new_symbols_info, symbol_less);

    for (const auto &removed_symbol : removed_symbols) {
      sqlite_db_.Delete(
          table_definition::Symbol().table,
          "WHERE Symbol.name = \"" + removed_symbol.symbol + "\"");
    }

    const auto added_symbols = ranges::views::set_difference(
        new_symbols_info, old_symbols_info, symbol_less);

    for (const auto &added_symbol : added_symbols) {
      InsertSymbolInfo(added_symbol);
    }

    const auto existing_symbols = ranges::views::set_intersection(
        new_symbols_info, old_symbols_info, symbol_less);
    const auto updated_symbols =
        ranges::views::set_difference(existing_symbols, old_symbols_info);

    for (const auto &updated_symbol : updated_symbols) {
      UpdateSymbolInfoInTable(updated_symbol);
    }

    cache_.UpdateFromDb(sqlite_db_);
  }

  [[nodiscard]] auto SelectSymbolPriceTicks(
      std::optional<int> limit = std::nullopt,
      const std::optional<Period> &period = std::nullopt,
      const std::optional<std::vector<SymbolName>> &symbols = std::nullopt)
      -> std::vector<SymbolPriceTick> {
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

    if (symbols.has_value() && !symbols->empty()) {
      if (add_and) {
        condition += " AND ";
      }

      auto symbol_ids = std::string{};

      for (const auto &symbol : *symbols) {
        const auto symbol_id = cache_.GetSymbolIdBySymbol(symbol);

        if (!symbol_id.has_value()) {
          Logger().error("Symbol not found in DB {}", symbol);
          continue;
        }

        symbol_ids += std::to_string(*symbol_id);

        if (const auto not_last_item = &symbol != &symbols->back()) {
          symbol_ids += ", ";
        }
      }

      condition += "SymbolPriceTick.symbol_id IN (" + symbol_ids + ")";

      add_and = true;
    }

    if (!condition.empty()) {
      query += " WHERE " + condition;
    }

    if (limit.has_value()) {
      query += " LIMIT " + std::to_string(*limit);
    }

    const auto rows =
        *sqlite_db_.Select(query, table_definition::SymbolPriceTick().columns);

    return rows | ranges::views::transform([this](const auto &row) {
             const auto symbol = cache_.GetSymbolBySymbolId(
                 *row.GetCellValueInt64("symbol_id"));
             ABSL_ASSERT(symbol.has_value() && "Logic error");

             return SymbolPriceTick{
                 .symbol = *symbol,
                 .time =
                     std::chrono::milliseconds{*row.GetCellValueInt64("time")},
                 .buy_price = *row.GetCellValueDouble("buy_price"),
                 .sell_price = *row.GetCellValueDouble("sell_price")};
           }) |
           ranges::to_vector;
  }

  void InsertSymbolPriceTick(const SymbolPriceTick &symbol_price_tick) {
    const auto symbol_id = cache_.GetSymbolIdBySymbol(symbol_price_tick.symbol);

    if (!symbol_id.has_value()) {
      Logger().error("Symbol not found in DB {}", symbol_price_tick.symbol);
      return;
    }

    sqlite_db_.Insert(
        table_definition::SymbolPriceTick().table,
        db::Row{.cells = {
                    db::Cell{.column_name = "symbol_id", .value = {*symbol_id}},
                    db::Cell{.column_name = "time",
                             .value = {symbol_price_tick.time.count()}},
                    db::Cell{.column_name = "buy_price",
                             .value = {symbol_price_tick.buy_price}},
                    db::Cell{.column_name = "sell_price",
                             .value = {symbol_price_tick.sell_price}}}});
  }

 private:
  class Cache {
   public:
    [[nodiscard]] auto GetAssetIdByAsset(const std::string &asset) const
        -> std::optional<int64_t> {
      return GetValue(asset_to_asset_id_map_, asset);
    }

    [[nodiscard]] auto GetSymbolIdBySymbol(const SymbolName &symbol) const
        -> std::optional<int64_t> {
      return GetValue(symbol_to_symbol_id_map_, symbol);
    }

    [[nodiscard]] auto GetSymbolBySymbolId(int64_t symbol_id) const
        -> std::optional<SymbolName> {
      return GetValue(symbol_id_to_symbol_map_, symbol_id);
    }

    void UpdateFromDb(const db::Db &db) {
      UpdateAssetMapsFromDb(db);
      UpdateSymbolMapsFromDb(db);
    }

   private:
    void UpdateAssetMapsFromDb(const db::Db &db) {
      const auto rows =
          *db.Select("SELECT * FROM Asset", table_definition::Symbol().columns);

      asset_to_asset_id_map_.clear();

      for (const auto &row : rows) {
        const auto id = *row.GetCellValueInt64("id");
        const auto name = *row.GetCellValueString("name");

        asset_to_asset_id_map_[name] = id;
      }
    }

    void UpdateSymbolMapsFromDb(const db::Db &db) {
      static constinit const auto kColumnNames = std::array{"id", "name"};

      const auto columns = table_definition::Symbol().columns |
                           ranges::views::filter([](const auto &column) {
                             return ranges::contains(kColumnNames, column.name);
                           }) |
                           ranges::to_vector;
      const auto rows =
          *db.Select("SELECT Symbol.id, Symbol.name FROM Symbol", {columns});

      symbol_to_symbol_id_map_.clear();
      symbol_id_to_symbol_map_.clear();

      for (const auto &row : rows) {
        const auto id = *row.GetCellValueInt64("id");
        const auto name = *row.GetCellValueString("name");

        symbol_to_symbol_id_map_[name] = id;
        symbol_id_to_symbol_map_[id] = name;
      }
    }

    template <typename Key, typename Value>
    [[nodiscard]] static auto GetValue(const std::map<Key, Value> &map,
                                       const Key &key) -> std::optional<Value> {
      auto value = map.find(key);

      if (value == map.end()) {
        return std::nullopt;
      }

      return std::move(value->second);
    }

    std::map<std::string, int64_t> asset_to_asset_id_map_{};
    std::map<SymbolName, int64_t> symbol_to_symbol_id_map_{};
    std::map<int64_t, SymbolName> symbol_id_to_symbol_map_{};
  };

  db::sqlite::SqliteDb sqlite_db_;
  Cache cache_{};
};

FinanceDb::FinanceDb(std::string_view uri)
    : impl_{std::make_unique<Impl>(uri)} {}

FinanceDb::FinanceDb(FinanceDb &&) noexcept = default;

auto FinanceDb::operator=(FinanceDb &&) noexcept -> FinanceDb & = default;

FinanceDb::~FinanceDb() = default;

auto FinanceDb::SelectAssets() const -> std::vector<std::string> {
  return impl_->SelectAssets();
}

void FinanceDb::UpdateAssets(std::vector<std::string> assets) {
  impl_->UpdateAssets(std::move(assets));
}

auto FinanceDb::SelectSymbols() const -> std::vector<SymbolName> {
  return impl_->SelectSymbols();
}

auto FinanceDb::SelectSymbolsInfo() const -> std::vector<SymbolInfo> {
  return impl_->SelectSymbolsInfo();
}

void FinanceDb::UpdateSymbolsInfo(std::vector<SymbolInfo> symbols_info) {
  impl_->UpdateSymbolsInfo(std::move(symbols_info));
}

auto FinanceDb::SelectSymbolPriceTicks(
    std::optional<int> limit, const std::optional<Period> &period,
    const std::optional<std::vector<SymbolName>> &symbols) const
    -> std::vector<SymbolPriceTick> {
  return impl_->SelectSymbolPriceTicks(limit, period, symbols);
}

void FinanceDb::InsertSymbolPriceTick(
    const SymbolPriceTick &symbol_price_tick) {
  impl_->InsertSymbolPriceTick(symbol_price_tick);
}
}  // namespace stonks::finance