#include "stonks_db.h"

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

#include "not_null.hpp"
#include "sqldb_i_db.h"
#include "sqldb_i_factory.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_rows.h"
#include "sqldb_value.h"
#include "stonks_cache.h"
#include "stonks_prepared_statements.h"
#include "stonks_table_definitions.h"

namespace stonks {
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

auto SymbolLess(const SymbolInfo &left, const SymbolInfo &right) -> bool {
  return left.symbol < right.symbol;
}

auto SymbolEqual(const SymbolInfo &left, const SymbolInfo &right) -> bool {
  return left.symbol == right.symbol;
}
}  // namespace

Db::Db(std::string_view file_path, const sqldb::IFactory &db_factory)
    : db_{db_factory.LoadDbFromFile(file_path)},
      query_builder_{db_factory.CreateQueryBuilder()},
      prepared_statements_{cpp::assume_not_null(
          std::make_shared<db::PreparedStatements>(db_, query_builder_))},
      cache_{prepared_statements_} {
  CreateTablesIfNotExist();
  cache_.Update();
}

Db::Db(Db &&) noexcept = default;

auto Db::operator=(Db &&) noexcept -> Db & = default;

Db::~Db() = default;

auto Db::SelectAssets() const -> std::vector<std::string> {
  auto rows = prepared_statements_->SelectAssets().Execute();
  auto &name_values = rows.GetColumnValues("name");
  return name_values | ranges::views::transform([](auto &asset) {
           return std::move(asset.GetString());
         }) |
         ranges::to_vector;
}

void Db::UpdateAssets(std::vector<std::string> assets) {
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

auto Db::SelectSymbols() const -> std::vector<SymbolName> {
  auto rows = prepared_statements_->SelectSymbols().Execute();
  auto &name_values = rows.GetColumnValues("name");
  return name_values | ranges::views::transform([](auto &asset) {
           return std::move(asset.GetString());
         }) |
         ranges::to_vector;
}

auto Db::SelectSymbolsInfo() const -> std::vector<SymbolInfo> {
  auto rows = prepared_statements_->SelectSymbolsInfo().Execute();

  auto &symbol = rows.GetColumnValues("name");
  auto &min_base_amount = rows.GetColumnValues("min_base_amount");
  auto &min_quote_amount = rows.GetColumnValues("min_quote_amount");
  auto &base_step = rows.GetColumnValues("base_step");
  auto &quote_step = rows.GetColumnValues("quote_step");
  auto &base_asset = rows.GetColumnValues("base_asset");
  auto &quote_asset = rows.GetColumnValues("quote_asset");

  const auto num_rows = rows.GetSize();

  auto symbols_info = std::vector<SymbolInfo>{};
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

void Db::UpdateSymbolsInfo(std::vector<SymbolInfo> symbols_info) {
  symbols_info |=
      ranges::actions::sort(SymbolLess) | ranges::actions::unique(SymbolEqual);
  const auto old_symbols_info = SelectSymbolsInfo() |
                                ranges::actions::sort(SymbolLess) |
                                ranges::actions::unique(SymbolEqual);
  const auto &new_symbols_info = symbols_info;

  const auto removed_symbols = ranges::views::set_difference(
      old_symbols_info, new_symbols_info, SymbolLess);

  for (const auto &removed_symbol : removed_symbols) {
    prepared_statements_->DeleteSymbolInfo().Execute(
        {sqldb::Value{removed_symbol.symbol}});
  }

  const auto added_symbols = ranges::views::set_difference(
      new_symbols_info, old_symbols_info, SymbolLess);

  for (const auto &added_symbol : added_symbols) {
    InsertSymbolInfo(added_symbol);
  }

  const auto existing_symbols = ranges::views::set_intersection(
      new_symbols_info, old_symbols_info, SymbolLess);
  const auto updated_symbols =
      ranges::views::set_difference(existing_symbols, old_symbols_info);

  for (const auto &updated_symbol : updated_symbols) {
    UpdateSymbolInfo(updated_symbol);
  }

  cache_.Update();
}

auto Db::SelectSymbolPriceTicks(const SymbolName *symbol, const Period *period,
                                const int *limit) const
    -> std::vector<SymbolPriceTick> {
  const auto *statement = symbol != nullptr
                              ? &prepared_statements_->SelectSymbolPriceTicks()
                              : &prepared_statements_->SelectPriceTicks();
  const auto values = [this, symbol, period, limit]() {
    auto values = std::vector<sqldb::Value>{GetStartTime(period).count(),
                                            GetEndTime(period).count()};

    if (symbol != nullptr) {
      values.emplace_back(cache_.GetSymbolIdBySymbol(*symbol));
    }

    values.emplace_back((limit != nullptr) ? *limit
                                           : std::numeric_limits<int>::max());
    return values;
  }();

  auto rows = statement->Execute(values);

  auto &symbol_id = rows.GetColumnValues("symbol_id");
  auto &time = rows.GetColumnValues("time");
  auto &buy_price = rows.GetColumnValues("buy_price");
  auto &sell_price = rows.GetColumnValues("sell_price");

  const auto num_rows = rows.GetSize();

  auto price_ticks = std::vector<SymbolPriceTick>{};
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

void Db::InsertSymbolPriceTick(const SymbolPriceTick &symbol_price_tick) {
  prepared_statements_->InsertPriceTick().Execute(
      {cache_.GetSymbolIdBySymbol(symbol_price_tick.symbol),
       symbol_price_tick.time.count(), symbol_price_tick.buy_price,
       symbol_price_tick.sell_price});
}

void Db::CreateTablesIfNotExist() {
  db_->PrepareStatement(query_builder_->BuildCreateTableIfNotExistsQuery(
                            db::table_definitions::Asset()))
      ->Execute();
  db_->PrepareStatement(query_builder_->BuildCreateTableIfNotExistsQuery(
                            db::table_definitions::Symbol()))
      ->Execute();
  db_->PrepareStatement(query_builder_->BuildCreateTableIfNotExistsQuery(
                            db::table_definitions::SymbolPriceTick()))
      ->Execute();
}

void Db::InsertSymbolInfo(const SymbolInfo &symbol_info) {
  prepared_statements_->InsertSymbolInfo().Execute(
      {sqldb::Value{symbol_info.symbol},
       cache_.GetAssetIdByAsset(symbol_info.base_asset),
       cache_.GetAssetIdByAsset(symbol_info.quote_asset),
       symbol_info.min_base_amount, symbol_info.min_quote_amount,
       symbol_info.base_step, symbol_info.quote_step});
}

void Db::UpdateSymbolInfo(const SymbolInfo &symbol_info) {
  prepared_statements_->UpdateSymbolInfo().Execute(
      {cache_.GetAssetIdByAsset(symbol_info.base_asset),
       cache_.GetAssetIdByAsset(symbol_info.quote_asset),
       symbol_info.min_base_amount, symbol_info.min_quote_amount,
       symbol_info.base_step, symbol_info.quote_step,
       sqldb::Value{symbol_info.symbol}});
}
}  // namespace stonks