#include "prepared_statements.h"

#include <gsl/assert>
#include <iterator>
#include <memory>
#include <range/v3/action/insert.hpp>
#include <range/v3/all.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/concat.hpp>

#include "sqldb_query_builder.h"
#include "sqldb_query_builder_facade.h"
#include "sqldb_row_definition.h"
#include "sqldb_select_query_builder.h"
#include "table_definitions.h"

namespace stonks::finance {
namespace {
auto InitSelectPriceTicksQuery(
    const sqldb::QueryBuilderFacade& query_builder_facade)
    -> sqldb::SelectQueryBuilder {
  return query_builder_facade.Select()
      .AllColumns()
      .FromTable(table_definitions::SymbolPriceTick())
      .Where("SymbolPriceTick.time >= ?")
      .And("SymbolPriceTick.time <= ?")
      .Limited();
}
}  // namespace

PreparedStatements::PreparedStatements(
    std::shared_ptr<sqldb::IDb> db,
    std::shared_ptr<sqldb::IQueryBuilder> query_builder)
    : db_{std::move(db)},
      query_builder_{std::move(query_builder)},
      query_builder_facade_{
          std::make_unique<sqldb::QueryBuilderFacade>(query_builder_)} {
  Ensures(db_ != nullptr);
  Ensures(query_builder_ != nullptr);
  Ensures(query_builder_facade_ != nullptr);
}

auto PreparedStatements::SelectAssets() -> sqldb::ISelectStatement& {
  if (select_assets_ == nullptr) {
    const auto& table = table_definitions::Asset();
    const auto columns = table.GetColumnDefinitions({"name"});
    auto query = query_builder_facade_->Select()
                     .Columns(columns)
                     .FromTable(table)
                     .Build();
    select_assets_ = db_->PrepareStatement(query, columns);
  }

  Ensures(select_assets_ != nullptr);
  return *select_assets_;
}

auto PreparedStatements::SelectAssetsWithIds() -> sqldb::ISelectStatement& {
  if (select_assets_with_ids_ == nullptr) {
    const auto& table = table_definitions::Asset();
    auto query =
        query_builder_facade_->Select().AllColumns().FromTable(table).Build();
    select_assets_with_ids_ = db_->PrepareStatement(query, table);
  }

  Ensures(select_assets_with_ids_ != nullptr);
  return *select_assets_with_ids_;
}

auto PreparedStatements::DeleteAsset() -> sqldb::IUpdateStatement& {
  if (delete_asset_ == nullptr) {
    auto query = query_builder_facade_->Delete()
                     .FromTable(table_definitions::Asset())
                     .Where("Asset.name = ?")
                     .Build();
    delete_asset_ = db_->PrepareStatement(std::move(query));
  }

  Ensures(delete_asset_ != nullptr);
  return *delete_asset_;
}

auto PreparedStatements::InsertAsset() -> sqldb::IUpdateStatement& {
  if (insert_asset_ == nullptr) {
    auto query = query_builder_facade_->Insert()
                     .IntoTable(table_definitions::Asset())
                     .IntoColumns({"name"})
                     .Build();
    insert_asset_ = db_->PrepareStatement(std::move(query));
  }

  Ensures(insert_asset_ != nullptr);
  return *insert_asset_;
}

auto PreparedStatements::SelectSymbols() -> sqldb::ISelectStatement& {
  if (select_symbols_ == nullptr) {
    const auto& table = table_definitions::Symbol();
    const auto columns = table.GetColumnDefinitions({"name"});
    auto query = query_builder_facade_->Select()
                     .Columns(columns)
                     .FromTable(table)
                     .Build();
    select_symbols_ = db_->PrepareStatement(std::move(query), columns);
  }

  Ensures(select_symbols_ != nullptr);
  return *select_symbols_;
}

auto PreparedStatements::SelectSymbolsWithIds() -> sqldb::ISelectStatement& {
  if (select_symbols_with_ids_ == nullptr) {
    const auto& table = table_definitions::Symbol();
    const auto columns = table.GetColumnDefinitions({"id", "name"});
    auto query = query_builder_facade_->Select()
                     .Columns(columns)
                     .FromTable(table)
                     .Build();
    select_symbols_with_ids_ = db_->PrepareStatement(std::move(query), columns);
  }

  Ensures(select_symbols_with_ids_ != nullptr);
  return *select_symbols_with_ids_;
}

auto PreparedStatements::SelectSymbolsInfo() -> sqldb::ISelectStatement& {
  if (select_symbols_info_ == nullptr) {
    auto symbol_columns = table_definitions::Symbol().GetColumnDefinitions(
        {"name", "min_base_amount", "min_quote_amount", "base_step",
         "quote_step"});
    auto asset_columns =
        table_definitions::Asset().GetColumnDefinitions({"name", "name"});

    symbol_columns.insert(symbol_columns.end(),
                          std::move_iterator(asset_columns.begin()),
                          std::move_iterator(asset_columns.end()));

    const auto& columns = symbol_columns;
    const auto* query =
        "SELECT "
        "Symbol.name, "
        "Symbol.min_base_amount, "
        "Symbol.min_quote_amount, "
        "Symbol.base_step, "
        "Symbol.quote_step "
        "BaseAsset.name AS base_asset, "
        "QuoteAsset.name AS quote_asset, "
        "FROM Symbol "
        "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
        "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id";
    select_symbols_info_ = db_->PrepareStatement(query, columns);
  }

  Ensures(select_symbols_info_ != nullptr);
  return *select_symbols_info_;
}

auto PreparedStatements::InsertSymbolInfo() -> sqldb::IUpdateStatement& {
  if (insert_symbol_info_ == nullptr) {
    auto query = query_builder_facade_->Insert()
                     .WholeRow()
                     .IntoTable(table_definitions::Symbol())
                     .Build();
    insert_symbol_info_ = db_->PrepareStatement(std::move(query));
  }

  Ensures(insert_symbol_info_ != nullptr);
  return *insert_symbol_info_;
}

auto PreparedStatements::UpdateSymbolInfo() -> sqldb::IUpdateStatement& {
  if (update_symbol_info_ == nullptr) {
    auto query =
        query_builder_facade_->Update()
            .Columns({"base_asset_id", "quote_asset_id", "min_base_amount",
                      "min_quote_amount", "base_step", "quote_step"})
            .OfTable(table_definitions::Symbol())
            .Where("Symbol.name = ?")
            .Build();
    update_symbol_info_ = db_->PrepareStatement(std::move(query));
  }

  Ensures(update_symbol_info_ != nullptr);
  return *update_symbol_info_;
}

auto PreparedStatements::DeleteSymbolInfo() -> sqldb::IUpdateStatement& {
  if (delete_symbol_info_ == nullptr) {
    auto query = query_builder_facade_->Delete()
                     .FromTable(table_definitions::Symbol())
                     .Where("Symbol.name = ?")
                     .Build();
    delete_symbol_info_ = db_->PrepareStatement(std::move(query));
  }

  Ensures(delete_symbol_info_ != nullptr);
  return *delete_symbol_info_;
}

auto PreparedStatements::SelectPriceTicks() -> sqldb::ISelectStatement& {
  if (select_price_ticks_ == nullptr) {
    auto query = InitSelectPriceTicksQuery(*query_builder_facade_).Build();
    select_price_ticks_ = db_->PrepareStatement(
        std::move(query), table_definitions::SymbolPriceTick());
  }

  Ensures(select_price_ticks_ != nullptr);
  return *select_price_ticks_;
}

auto PreparedStatements::SelectSymbolPriceTicks() -> sqldb::ISelectStatement& {
  if (select_symbol_price_ticks_ == nullptr) {
    auto query = InitSelectPriceTicksQuery(*query_builder_facade_)
                     .And("SymbolPriceTick.symbol_id = ?")
                     .Build();
    select_symbol_price_ticks_ = db_->PrepareStatement(
        std::move(query), table_definitions::SymbolPriceTick());
  }

  Ensures(select_symbol_price_ticks_ != nullptr);
  return *select_symbol_price_ticks_;
}

auto PreparedStatements::InsertPriceTick() -> sqldb::IUpdateStatement& {
  if (insert_price_tick_ == nullptr) {
    auto query = query_builder_facade_->Insert()
                     .WholeRow()
                     .IntoTable(table_definitions::SymbolPriceTick())
                     .Build();
    insert_price_tick_ = db_->PrepareStatement(std::move(query));
  }

  Ensures(insert_price_tick_ != nullptr);
  return *insert_price_tick_;
}
}  // namespace stonks::finance