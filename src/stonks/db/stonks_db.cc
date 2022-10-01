#include "stonks_db.h"

#include <absl/time/time.h>

#include <compare>
#include <limits>
#include <memory>
#include <range/v3/action/action.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/functional/comparisons.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/iterator/default_sentinel.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>
#include <vector>

#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_typed_struct.h"
#include "not_null.hpp"
#include "sqldb_as_values.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_query_builder_facade.h"
#include "sqldb_rows.h"
#include "sqldb_value.h"
#include "stonks_db_cache.h"
#include "stonks_db_prepared_statements.h"
#include "stonks_db_tables.h"

namespace stonks {
namespace {
[[nodiscard]] auto GetStartTime(const Period *period) -> absl::Time {
  if ((period != nullptr) && period->start_time.has_value()) {
    return *period->start_time;
  }

  return absl::InfinitePast();
}

[[nodiscard]] auto GetEndTime(const Period *period) -> absl::Time {
  if ((period != nullptr) && period->end_time.has_value()) {
    return *period->end_time;
  }

  return absl::InfiniteFuture();
}

[[nodiscard]] auto SymbolLess(const SymbolInfo &left, const SymbolInfo &right)
    -> bool {
  return left.symbol < right.symbol;
}

[[nodiscard]] auto SymbolEqual(const SymbolInfo &left, const SymbolInfo &right)
    -> bool {
  return left.symbol == right.symbol;
}
}  // namespace

Db::Db(cpp::NnUp<sqldb::IDb> db,
       cpp::NnSp<di::IFactory<sqldb::IQueryBuilder>> query_builder_factory)
    : db_{std::move(db)},
      query_builder_{cpp::AssumeNn(query_builder_factory->create())},
      prepared_statements_{cpp::MakeNnSp<db::PreparedStatements>(
          db_, sqldb::QueryBuilderFacade{std::move(query_builder_factory)})},
      cache_{prepared_statements_} {
  CreateTablesIfNotExist();
  cache_.Update();
}

auto Db::SelectAssets() const -> std::vector<std::string> {
  auto rows = prepared_statements_->SelectAssets().Execute();
  auto &name_values = rows.GetColumnValues({"name"});
  return name_values | ranges::views::transform([](auto &asset) {
           return std::move(asset.GetString());
         }) |
         ranges::to_vector;
}

void Db::UpdateAssets(const std::vector<std::string> &assets) {
  const auto old_assets =
      SelectAssets() | ranges::actions::sort | ranges::actions::unique;
  const auto new_assets = std::vector<std::string>{assets} |
                          ranges::actions::sort | ranges::actions::unique;

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
  auto &name_values = rows.GetColumnValues({"name"});
  return name_values | ranges::views::transform([](auto &asset) {
           return SymbolName{std::move(asset.GetString())};
         }) |
         ranges::to_vector;
}

auto Db::SelectSymbolsInfo() const -> std::vector<SymbolInfo> {
  auto rows = prepared_statements_->SelectSymbolsInfo().Execute();

  auto &symbol = rows.GetColumnValues({"name"});
  const auto &min_base_amount = rows.GetColumnValues({"min_base_amount"});
  const auto &min_quote_amount = rows.GetColumnValues({"min_quote_amount"});
  const auto &base_step = rows.GetColumnValues({"base_step"});
  const auto &quote_step = rows.GetColumnValues({"quote_step"});
  auto &base_asset = rows.GetColumnValues({"base_asset"});
  auto &quote_asset = rows.GetColumnValues({"quote_asset"});

  const auto num_rows = rows.GetSize();

  auto symbols_info = std::vector<SymbolInfo>{};
  symbols_info.reserve(num_rows);

  for (auto i = 0; i < num_rows; ++i) {
    symbols_info.emplace_back(
        SymbolInfo{.symbol = {std::move(symbol[i].GetString())},
                   .base_asset = std::move(base_asset[i].GetString()),
                   .quote_asset = std::move(quote_asset[i].GetString()),
                   .min_base_amount = min_base_amount[i].GetDouble(),
                   .min_quote_amount = min_quote_amount[i].GetDouble(),
                   .base_step = base_step[i].GetDouble(),
                   .quote_step = quote_step[i].GetDouble()});
  }

  return symbols_info;
}

void Db::UpdateSymbolsInfo(const std::vector<SymbolInfo> &symbols_info) {
  const auto old_symbols_info = SelectSymbolsInfo() |
                                ranges::actions::sort(SymbolLess) |
                                ranges::actions::unique(SymbolEqual);
  const auto new_symbols_info = std::vector<SymbolInfo>{symbols_info} |
                                ranges::actions::sort(SymbolLess) |
                                ranges::actions::unique(SymbolEqual);

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
  auto values = [this, symbol, period, limit]() {
    auto values = sqldb::AsValues(absl::ToUnixMillis(GetStartTime(period)),
                                  absl::ToUnixMillis(GetEndTime(period)));

    if (symbol != nullptr) {
      values.emplace_back(cache_.GetSymbolIdBySymbol(*symbol));
    }

    values.emplace_back((limit != nullptr) ? *limit
                                           : std::numeric_limits<int>::max());
    return values;
  }();

  const auto rows = statement->Execute(std::move(values));

  const auto &symbol_id = rows.GetColumnValues({"symbol_id"});
  const auto &time = rows.GetColumnValues({"time"});
  const auto &buy_price = rows.GetColumnValues({"buy_price"});
  const auto &sell_price = rows.GetColumnValues({"sell_price"});

  const auto num_rows = rows.GetSize();

  auto price_ticks = std::vector<SymbolPriceTick>{};
  price_ticks.reserve(num_rows);

  for (auto i = 0; i < num_rows; ++i) {
    price_ticks.emplace_back(SymbolPriceTick{
        .symbol = cache_.GetSymbolBySymbolId(symbol_id[i].GetInt64()),
        .time = absl::FromUnixMillis(time[i].GetInt64()),
        .buy_price = buy_price[i].GetDouble(),
        .sell_price = sell_price[i].GetDouble()});
  }

  return price_ticks;
}

void Db::InsertSymbolPriceTick(const SymbolPriceTick &symbol_price_tick) {
  prepared_statements_->InsertPriceTick().Execute(sqldb::AsValues(
      cache_.GetSymbolIdBySymbol(symbol_price_tick.symbol),
      absl::ToUnixMillis(symbol_price_tick.time), symbol_price_tick.buy_price,
      symbol_price_tick.sell_price));
}

void Db::CreateTablesIfNotExist() {
  db_->PrepareStatement(
         query_builder_->BuildCreateTableIfNotExistsQuery(db::tables::Asset()))
      ->Execute();
  db_->PrepareStatement(
         query_builder_->BuildCreateTableIfNotExistsQuery(db::tables::Symbol()))
      ->Execute();
  db_->PrepareStatement(query_builder_->BuildCreateTableIfNotExistsQuery(
                            db::tables::SymbolPriceTick()))
      ->Execute();
}

void Db::InsertSymbolInfo(const SymbolInfo &symbol_info) {
  prepared_statements_->InsertSymbolInfo().Execute(sqldb::AsValues(
      symbol_info.symbol, cache_.GetAssetIdByAsset(symbol_info.base_asset),
      cache_.GetAssetIdByAsset(symbol_info.quote_asset),
      symbol_info.min_base_amount, symbol_info.min_quote_amount,
      symbol_info.base_step, symbol_info.quote_step));
}

void Db::UpdateSymbolInfo(const SymbolInfo &symbol_info) {
  prepared_statements_->UpdateSymbolInfo().Execute(sqldb::AsValues(
      cache_.GetAssetIdByAsset(symbol_info.base_asset),
      cache_.GetAssetIdByAsset(symbol_info.quote_asset),
      symbol_info.min_base_amount, symbol_info.min_quote_amount,
      symbol_info.base_step, symbol_info.quote_step, symbol_info.symbol));
}
}  // namespace stonks