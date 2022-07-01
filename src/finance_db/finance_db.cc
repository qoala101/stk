#include "finance_db.h"

#include <chrono>
#include <compare>
#include <concepts/concepts.hpp>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <range/v3/action/action.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/comparisons.hpp>
#include <range/v3/functional/compose.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/iterator/default_sentinel.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>
#include <vector>

#include "cache.h"
#include "finance_types.h"
#include "not_null.hpp"
#include "prepared_statements.h"
#include "sqldb_db.h"
#include "sqldb_db_factory.h"
#include "sqldb_query_builder.h"
#include "sqldb_rows.h"
#include "sqldb_select_statement.h"
#include "sqldb_update_statement.h"
#include "sqldb_value.h"
#include "table_definitions.h"

namespace stonks::finance {
namespace {
auto GetStartTime(const Period *period) -> std::chrono::milliseconds {
  if ((period != nullptr) && period->start_time.has_value()) {
    return *period->start_time;
  }

  return std::chrono::milliseconds::min();
}

auto GetEndTime(const Period *period) -> std::chrono::milliseconds {
  if ((period != nullptr) && period->end_time.has_value()) {
    return *period->end_time;
  }

  return std::chrono::milliseconds::max();
}
}  // namespace

FinanceDb::FinanceDb(const sqldb::IDbFactory &db_factory,
                     std::string_view file_path)
    : db_{db_factory.LoadDbFromFile(file_path)},
      query_builder_{db_factory.CreateQueryBuilder()},
      prepared_statements_{cpp::assume_not_null(
          std::make_shared<PreparedStatements>(db_, query_builder_))},
      cache_{prepared_statements_} {
  CreateTablesIfNotExist();
  cache_.Update();
}

FinanceDb::FinanceDb(FinanceDb &&) noexcept = default;

auto FinanceDb::operator=(FinanceDb &&) noexcept -> FinanceDb & = default;

FinanceDb::~FinanceDb() = default;

auto FinanceDb::SelectAssets() const -> std::vector<std::string> {
  auto rows = prepared_statements_->SelectAssets().Execute();
  auto &name_values = rows.GetColumnValues("name");
  return name_values | ranges::views::transform([](auto &asset) {
           return std::move(asset.GetString());
         }) |
         ranges::to_vector;
}

void FinanceDb::UpdateAssets(std::vector<std::string> assets) {
  assets |= ranges::actions::sort | ranges::actions::unique;
  const auto old_assets =
      SelectAssets() | ranges::actions::sort | ranges::actions::unique;
  const auto &new_assets = assets;

  const auto removed_assets =
      ranges::views::set_difference(old_assets, new_assets);

  for (const auto &removed_asset : removed_assets) {
    prepared_statements_->DeleteAsset().Execute({sqldb::Value{removed_asset}});
  }

  const auto added_assets =
      ranges::views::set_difference(new_assets, old_assets);

  for (const auto &added_asset : added_assets) {
    prepared_statements_->InsertAsset().Execute({sqldb::Value{added_asset}});
  }

  cache_.Update();
}

auto FinanceDb::SelectSymbols() const -> std::vector<SymbolName> {
  auto rows = prepared_statements_->SelectSymbols().Execute();
  auto &name_values = rows.GetColumnValues("name");
  return name_values | ranges::views::transform([](auto &asset) {
           return std::move(asset.GetString());
         }) |
         ranges::to_vector;
}

auto FinanceDb::SelectSymbolsInfo() const -> std::vector<SymbolInfo> {
  auto rows = prepared_statements_->SelectSymbolsInfo().Execute();

  auto &symbol = rows.GetColumnValues("name");
  auto &min_base_amount = rows.GetColumnValues("min_base_amount");
  auto &min_quote_amount = rows.GetColumnValues("min_quote_amount");
  auto &base_step = rows.GetColumnValues("base_step");
  auto &quote_step = rows.GetColumnValues("quote_step");
  auto &base_asset = rows.GetColumnValues("base_asset");
  auto &quote_asset = rows.GetColumnValues("quote_asset");

  auto symbols_info = std::vector<SymbolInfo>{};
  const auto num_rows = rows.GetSize();
  symbols_info.reserve(num_rows);

  for (auto i = 0; i < num_rows; ++i) {
    symbols_info.emplace_back(
        SymbolInfo{.symbol = std::move(symbol[i].GetString()),
                   .base_asset = std::move(base_asset[i].GetString()),
                   .quote_asset = std::move(quote_asset[i].GetString()),
                   .min_base_amount = min_base_amount[i].GetDouble(),
                   .min_quote_amount = min_quote_amount[i].GetDouble(),
                   .base_step = base_step[i].GetDouble(),
                   .quote_step = quote_step[i].GetDouble()});
  }

  return symbols_info;
}

void FinanceDb::UpdateSymbolsInfo(std::vector<SymbolInfo> symbols_info) {
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
    prepared_statements_->DeleteSymbolInfo().Execute(
        {sqldb::Value{removed_symbol.symbol}});
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
    UpdateSymbolInfo(updated_symbol);
  }

  cache_.Update();
}

auto FinanceDb::SelectSymbolPriceTicks(const SymbolName *symbol,
                                       const Period *period,
                                       const int *limit) const
    -> std::vector<SymbolPriceTick> {
  auto *statement = (sqldb::ISelectStatement *){};
  auto values = std::vector<sqldb::Value>{
      GetStartTime(period).count(), GetEndTime(period).count(),
      (limit != nullptr) ? *limit : std::numeric_limits<int>::max()};

  if (symbol != nullptr) {
    statement = &prepared_statements_->SelectSymbolPriceTicks();
    values.emplace_back(cache_.GetSymbolIdBySymbol(*symbol));
  } else {
    statement = &prepared_statements_->SelectPriceTicks();
  }

  auto rows = statement->Execute(values);

  auto &symbol_id = rows.GetColumnValues("symbol_id");
  auto &time = rows.GetColumnValues("time");
  auto &buy_price = rows.GetColumnValues("buy_price");
  auto &sell_price = rows.GetColumnValues("sell_price");

  auto price_ticks = std::vector<SymbolPriceTick>{};
  const auto num_rows = rows.GetSize();
  price_ticks.reserve(num_rows);

  for (auto i = 0; i < num_rows; ++i) {
    price_ticks.emplace_back(SymbolPriceTick{
        .symbol = cache_.GetSymbolBySymbolId(symbol_id[i].GetInt64()),
        .time = std::chrono::milliseconds{time[i].GetInt64()},
        .buy_price = buy_price[i].GetDouble(),
        .sell_price = sell_price[i].GetDouble()});
  }

  return price_ticks;
}

void FinanceDb::InsertSymbolPriceTick(
    const SymbolPriceTick &symbol_price_tick) {
  prepared_statements_->InsertPriceTick().Execute(
      {cache_.GetSymbolIdBySymbol(symbol_price_tick.symbol),
       symbol_price_tick.time.count(), symbol_price_tick.buy_price,
       symbol_price_tick.sell_price});
}

void FinanceDb::CreateTablesIfNotExist() {
  db_->PrepareStatement(query_builder_->BuildCreateTableIfNotExistsQuery(
                            table_definitions::Asset()))
      ->Execute();
  db_->PrepareStatement(query_builder_->BuildCreateTableIfNotExistsQuery(
                            table_definitions::Symbol()))
      ->Execute();
  db_->PrepareStatement(query_builder_->BuildCreateTableIfNotExistsQuery(
                            table_definitions::SymbolPriceTick()))
      ->Execute();
}

void FinanceDb::InsertSymbolInfo(const SymbolInfo &symbol_info) {
  prepared_statements_->InsertSymbolInfo().Execute(
      {sqldb::Value{symbol_info.symbol},
       cache_.GetAssetIdByAsset(symbol_info.base_asset),
       cache_.GetAssetIdByAsset(symbol_info.quote_asset),
       symbol_info.min_base_amount, symbol_info.min_quote_amount,
       symbol_info.base_step, symbol_info.quote_step});
}

void FinanceDb::UpdateSymbolInfo(const SymbolInfo &symbol_info) {
  prepared_statements_->UpdateSymbolInfo().Execute(
      {cache_.GetAssetIdByAsset(symbol_info.base_asset),
       cache_.GetAssetIdByAsset(symbol_info.quote_asset),
       symbol_info.min_base_amount, symbol_info.min_quote_amount,
       symbol_info.base_step, symbol_info.quote_step,
       sqldb::Value{symbol_info.symbol}});
}
}  // namespace stonks::finance