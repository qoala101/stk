#include "stonks_prepared_statements.h"

#include <gsl/assert>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "not_null.hpp"
#include "sqldb_delete_query_builder.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_insert_query_builder.h"
#include "sqldb_query_builder_facade.h"
#include "sqldb_select_query_builder.h"
#include "sqldb_types.h"
#include "sqldb_update_query_builder.h"
#include "stonks_table_definitions.h"

namespace stonks::db {
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
    cpp::not_null<std::shared_ptr<sqldb::IDb>> db,
    cpp::not_null<std::shared_ptr<sqldb::IQueryBuilder>> query_builder)
    : db_{std::move(db)},
      query_builder_{std::move(query_builder)},
      query_builder_facade_{cpp::assume_not_null(
          std::make_unique<sqldb::QueryBuilderFacade>(query_builder_))} {}

auto PreparedStatements::SelectAssets() const
    -> const sqldb::ISelectStatement& {
  if (select_assets_ == nullptr) {
    const auto& table = table_definitions::Asset();
    const auto columns = table.GetColumnDefinitions({"name"});
    auto query = query_builder_facade_->Select()
                     .Columns(columns)
                     .FromTable(table)
                     .Build();
    select_assets_ = db_->PrepareStatement(query, columns).as_nullable();
  }

  Ensures(select_assets_ != nullptr);
  return *select_assets_;
}

auto PreparedStatements::SelectAssetsWithIds() const
    -> const sqldb::ISelectStatement& {
  if (select_assets_with_ids_ == nullptr) {
    const auto& table = table_definitions::Asset();
    auto query =
        query_builder_facade_->Select().AllColumns().FromTable(table).Build();
    select_assets_with_ids_ = db_->PrepareStatement(query, table).as_nullable();
  }

  Ensures(select_assets_with_ids_ != nullptr);
  return *select_assets_with_ids_;
}

auto PreparedStatements::DeleteAsset() const -> const sqldb::IUpdateStatement& {
  if (delete_asset_ == nullptr) {
    auto query = query_builder_facade_->Delete()
                     .FromTable(table_definitions::Asset())
                     .Where("Asset.name = ?")
                     .Build();
    delete_asset_ = db_->PrepareStatement(std::move(query)).as_nullable();
  }

  Ensures(delete_asset_ != nullptr);
  return *delete_asset_;
}

auto PreparedStatements::InsertAsset() const -> const sqldb::IUpdateStatement& {
  if (insert_asset_ == nullptr) {
    auto query = query_builder_facade_->Insert()
                     .IntoTable(table_definitions::Asset())
                     .IntoColumns({"name"})
                     .Build();
    insert_asset_ = db_->PrepareStatement(std::move(query)).as_nullable();
  }

  Ensures(insert_asset_ != nullptr);
  return *insert_asset_;
}

auto PreparedStatements::SelectSymbols() const
    -> const sqldb::ISelectStatement& {
  if (select_symbols_ == nullptr) {
    const auto& table = table_definitions::Symbol();
    const auto columns = table.GetColumnDefinitions({"name"});
    auto query = query_builder_facade_->Select()
                     .Columns(columns)
                     .FromTable(table)
                     .Build();
    select_symbols_ =
        db_->PrepareStatement(std::move(query), columns).as_nullable();
  }

  Ensures(select_symbols_ != nullptr);
  return *select_symbols_;
}

auto PreparedStatements::SelectSymbolsWithIds() const
    -> const sqldb::ISelectStatement& {
  if (select_symbols_with_ids_ == nullptr) {
    const auto& table = table_definitions::Symbol();
    const auto columns = table.GetColumnDefinitions({"id", "name"});
    auto query = query_builder_facade_->Select()
                     .Columns(columns)
                     .FromTable(table)
                     .Build();
    select_symbols_with_ids_ =
        db_->PrepareStatement(std::move(query), columns).as_nullable();
  }

  Ensures(select_symbols_with_ids_ != nullptr);
  return *select_symbols_with_ids_;
}

auto PreparedStatements::SelectSymbolsInfo() const
    -> const sqldb::ISelectStatement& {
  if (select_symbols_info_ == nullptr) {
    auto symbol_columns = table_definitions::Symbol().GetColumnDefinitions(
        {"name", "min_base_amount", "min_quote_amount", "base_step",
         "quote_step"});

    const auto base_asset_column = []() {
      auto base_asset_column =
          table_definitions::Asset().GetColumnDefinition("name");
      base_asset_column.column = "base_asset";
      return base_asset_column;
    }();
    symbol_columns.emplace_back(cpp::assume_not_null(&base_asset_column));

    const auto quote_asset_column = []() {
      auto quote_asset_column =
          table_definitions::Asset().GetColumnDefinition("name");
      quote_asset_column.column = "quote_asset";
      return quote_asset_column;
    }();
    symbol_columns.emplace_back(cpp::assume_not_null(&quote_asset_column));

    const auto& columns = symbol_columns;
    const auto* query =
        "SELECT "
        "Symbol.name, "
        "Symbol.min_base_amount, "
        "Symbol.min_quote_amount, "
        "Symbol.base_step, "
        "Symbol.quote_step, "
        "BaseAsset.name AS base_asset, "
        "QuoteAsset.name AS quote_asset "
        "FROM Symbol "
        "JOIN Asset AS BaseAsset ON Symbol.base_asset_id = BaseAsset.id "
        "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id";

    select_symbols_info_ = db_->PrepareStatement(query, columns).as_nullable();
  }

  Ensures(select_symbols_info_ != nullptr);
  return *select_symbols_info_;
}

auto PreparedStatements::InsertSymbolInfo() const
    -> const sqldb::IUpdateStatement& {
  if (insert_symbol_info_ == nullptr) {
    const auto& table = table_definitions::Symbol();
    const auto columns = table.GetColumnDefinitions(
        {"name", "base_asset_id", "quote_asset_id", "min_base_amount",
         "min_quote_amount", "base_step", "quote_step"});
    auto query = query_builder_facade_->Insert()
                     .IntoTable(table)
                     .IntoColumns(columns)
                     .Build();
    insert_symbol_info_ = db_->PrepareStatement(std::move(query)).as_nullable();
  }

  Ensures(insert_symbol_info_ != nullptr);
  return *insert_symbol_info_;
}

auto PreparedStatements::UpdateSymbolInfo() const
    -> const sqldb::IUpdateStatement& {
  if (update_symbol_info_ == nullptr) {
    auto query =
        query_builder_facade_->Update()
            .Columns({"base_asset_id", "quote_asset_id", "min_base_amount",
                      "min_quote_amount", "base_step", "quote_step"})
            .OfTable(table_definitions::Symbol())
            .Where("Symbol.name = ?")
            .Build();
    update_symbol_info_ = db_->PrepareStatement(std::move(query)).as_nullable();
  }

  Ensures(update_symbol_info_ != nullptr);
  return *update_symbol_info_;
}

auto PreparedStatements::DeleteSymbolInfo() const
    -> const sqldb::IUpdateStatement& {
  if (delete_symbol_info_ == nullptr) {
    auto query = query_builder_facade_->Delete()
                     .FromTable(table_definitions::Symbol())
                     .Where("Symbol.name = ?")
                     .Build();
    delete_symbol_info_ = db_->PrepareStatement(std::move(query)).as_nullable();
  }

  Ensures(delete_symbol_info_ != nullptr);
  return *delete_symbol_info_;
}

auto PreparedStatements::SelectPriceTicks() const
    -> const sqldb::ISelectStatement& {
  if (select_price_ticks_ == nullptr) {
    auto query = InitSelectPriceTicksQuery(*query_builder_facade_).Build();
    select_price_ticks_ =
        db_->PrepareStatement(std::move(query),
                              table_definitions::SymbolPriceTick())
            .as_nullable();
  }

  Ensures(select_price_ticks_ != nullptr);
  return *select_price_ticks_;
}

auto PreparedStatements::SelectSymbolPriceTicks() const
    -> const sqldb::ISelectStatement& {
  if (select_symbol_price_ticks_ == nullptr) {
    auto query = InitSelectPriceTicksQuery(*query_builder_facade_)
                     .And("SymbolPriceTick.symbol_id = ?")
                     .Build();
    select_symbol_price_ticks_ =
        db_->PrepareStatement(std::move(query),
                              table_definitions::SymbolPriceTick())
            .as_nullable();
  }

  Ensures(select_symbol_price_ticks_ != nullptr);
  return *select_symbol_price_ticks_;
}

auto PreparedStatements::InsertPriceTick() const
    -> const sqldb::IUpdateStatement& {
  if (insert_price_tick_ == nullptr) {
    auto query = query_builder_facade_->Insert()
                     .WholeRow()
                     .IntoTable(table_definitions::SymbolPriceTick())
                     .Build();
    insert_price_tick_ = db_->PrepareStatement(std::move(query)).as_nullable();
  }

  Ensures(insert_price_tick_ != nullptr);
  return *insert_price_tick_;
}
}  // namespace stonks::db