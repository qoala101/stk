#include "stonks_db_prepared_statements.h"

#include <gsl/assert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "cpp_not_null.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_qbf_delete_query_builder.h"
#include "sqldb_qbf_insert_query_builder.h"
#include "sqldb_qbf_select_query_builder.h"
#include "sqldb_qbf_update_query_builder.h"
#include "sqldb_query_builder_facade.h"
#include "sqldb_types.h"
#include "stonks_db_tables.h"

namespace stonks::db {
namespace {
[[nodiscard]] auto InitSelectPriceTicksQuery(
    const sqldb::QueryBuilderFacade& query_builder_facade)
    -> sqldb::qbf::SelectQueryBuilder {
  return query_builder_facade.Select()
      .AllColumns()
      .FromTable(tables::SymbolPriceTick())
      .Where("SymbolPriceTick.time >= ?")
      .And("SymbolPriceTick.time <= ?")
      .Limited();
}
}  // namespace

PreparedStatements::PreparedStatements(
    cpp::NnSp<sqldb::IDb> db, cpp::NnSp<sqldb::IQueryBuilder> query_builder)
    : db_{std::move(db)},
      query_builder_{std::move(query_builder)},
      query_builder_facade_{
          cpp::MakeNnUp<sqldb::QueryBuilderFacade>(query_builder_)} {}

auto PreparedStatements::SelectAssets() const
    -> const sqldb::ISelectStatement& {
  if (select_assets_ == nullptr) {
    const auto& table = tables::Asset();
    const auto columns = table.GetColumnDefinitions({{"name"}});
    const auto query = query_builder_facade_->Select()
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
    const auto& table = tables::Asset();
    const auto query =
        query_builder_facade_->Select().AllColumns().FromTable(table).Build();
    select_assets_with_ids_ = db_->PrepareStatement(query, table).as_nullable();
  }

  Ensures(select_assets_with_ids_ != nullptr);
  return *select_assets_with_ids_;
}

auto PreparedStatements::DeleteAsset() const -> const sqldb::IUpdateStatement& {
  if (delete_asset_ == nullptr) {
    const auto query = query_builder_facade_->Delete()
                           .FromTable(tables::Asset())
                           .Where("Asset.name = ?")
                           .Build();
    delete_asset_ = db_->PrepareStatement(query).as_nullable();
  }

  Ensures(delete_asset_ != nullptr);
  return *delete_asset_;
}

auto PreparedStatements::InsertAsset() const -> const sqldb::IUpdateStatement& {
  if (insert_asset_ == nullptr) {
    const auto query = query_builder_facade_->Insert()
                           .IntoTable(tables::Asset())
                           .IntoColumns({{"name"}})
                           .Build();
    insert_asset_ = db_->PrepareStatement(query).as_nullable();
  }

  Ensures(insert_asset_ != nullptr);
  return *insert_asset_;
}

auto PreparedStatements::SelectSymbols() const
    -> const sqldb::ISelectStatement& {
  if (select_symbols_ == nullptr) {
    const auto& table = tables::Symbol();
    const auto columns = table.GetColumnDefinitions({{"name"}});
    const auto query = query_builder_facade_->Select()
                           .Columns(columns)
                           .FromTable(table)
                           .Build();
    select_symbols_ = db_->PrepareStatement(query, columns).as_nullable();
  }

  Ensures(select_symbols_ != nullptr);
  return *select_symbols_;
}

auto PreparedStatements::SelectSymbolsWithIds() const
    -> const sqldb::ISelectStatement& {
  if (select_symbols_with_ids_ == nullptr) {
    const auto& table = tables::Symbol();
    const auto columns = table.GetColumnDefinitions({{"id"}, {"name"}});
    const auto query = query_builder_facade_->Select()
                           .Columns(columns)
                           .FromTable(table)
                           .Build();
    select_symbols_with_ids_ =
        db_->PrepareStatement(query, columns).as_nullable();
  }

  Ensures(select_symbols_with_ids_ != nullptr);
  return *select_symbols_with_ids_;
}

auto PreparedStatements::SelectSymbolsInfo() const
    -> const sqldb::ISelectStatement& {
  if (select_symbols_info_ == nullptr) {
    auto symbol_columns =
        tables::Symbol().GetColumnDefinitions({{"name"},
                                               {"min_base_amount"},
                                               {"min_quote_amount"},
                                               {"base_step"},
                                               {"quote_step"}});

    const auto base_asset_column = []() {
      auto base_asset_column = tables::Asset().GetColumnDefinition({"name"});
      base_asset_column.column.value = "base_asset";
      return base_asset_column;
    }();
    symbol_columns.emplace_back(cpp::AssumeNn(&base_asset_column));

    const auto quote_asset_column = []() {
      auto quote_asset_column = tables::Asset().GetColumnDefinition({"name"});
      quote_asset_column.column.value = "quote_asset";
      return quote_asset_column;
    }();
    symbol_columns.emplace_back(cpp::AssumeNn(&quote_asset_column));

    const auto& columns = symbol_columns;
    auto query = sqldb::Query{
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
        "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id = QuoteAsset.id"};

    select_symbols_info_ =
        db_->PrepareStatement(std::move(query), columns).as_nullable();
  }

  Ensures(select_symbols_info_ != nullptr);
  return *select_symbols_info_;
}

auto PreparedStatements::InsertSymbolInfo() const
    -> const sqldb::IUpdateStatement& {
  if (insert_symbol_info_ == nullptr) {
    const auto& table = tables::Symbol();
    const auto columns = table.GetColumnDefinitions({{"name"},
                                                     {"base_asset_id"},
                                                     {"quote_asset_id"},
                                                     {"min_base_amount"},
                                                     {"min_quote_amount"},
                                                     {"base_step"},
                                                     {"quote_step"}});
    const auto query = query_builder_facade_->Insert()
                           .IntoTable(table)
                           .IntoColumns(columns)
                           .Build();
    insert_symbol_info_ = db_->PrepareStatement(query).as_nullable();
  }

  Ensures(insert_symbol_info_ != nullptr);
  return *insert_symbol_info_;
}

auto PreparedStatements::UpdateSymbolInfo() const
    -> const sqldb::IUpdateStatement& {
  if (update_symbol_info_ == nullptr) {
    const auto query = query_builder_facade_->Update()
                           .Columns({{"base_asset_id"},
                                     {"quote_asset_id"},
                                     {"min_base_amount"},
                                     {"min_quote_amount"},
                                     {"base_step"},
                                     {"quote_step"}})
                           .OfTable(tables::Symbol())
                           .Where("Symbol.name = ?")
                           .Build();
    update_symbol_info_ = db_->PrepareStatement(query).as_nullable();
  }

  Ensures(update_symbol_info_ != nullptr);
  return *update_symbol_info_;
}

auto PreparedStatements::DeleteSymbolInfo() const
    -> const sqldb::IUpdateStatement& {
  if (delete_symbol_info_ == nullptr) {
    const auto query = query_builder_facade_->Delete()
                           .FromTable(tables::Symbol())
                           .Where("Symbol.name = ?")
                           .Build();
    delete_symbol_info_ = db_->PrepareStatement(query).as_nullable();
  }

  Ensures(delete_symbol_info_ != nullptr);
  return *delete_symbol_info_;
}

auto PreparedStatements::SelectPriceTicks() const
    -> const sqldb::ISelectStatement& {
  if (select_price_ticks_ == nullptr) {
    const auto query =
        InitSelectPriceTicksQuery(*query_builder_facade_).Build();
    select_price_ticks_ =
        db_->PrepareStatement(query, tables::SymbolPriceTick()).as_nullable();
  }

  Ensures(select_price_ticks_ != nullptr);
  return *select_price_ticks_;
}

auto PreparedStatements::SelectSymbolPriceTicks() const
    -> const sqldb::ISelectStatement& {
  if (select_symbol_price_ticks_ == nullptr) {
    const auto query = InitSelectPriceTicksQuery(*query_builder_facade_)
                           .And("SymbolPriceTick.symbol_id = ?")
                           .Build();
    select_symbol_price_ticks_ =
        db_->PrepareStatement(query, tables::SymbolPriceTick()).as_nullable();
  }

  Ensures(select_symbol_price_ticks_ != nullptr);
  return *select_symbol_price_ticks_;
}

auto PreparedStatements::InsertPriceTick() const
    -> const sqldb::IUpdateStatement& {
  if (insert_price_tick_ == nullptr) {
    const auto query = query_builder_facade_->Insert()
                           .WholeRow()
                           .IntoTable(tables::SymbolPriceTick())
                           .Build();
    insert_price_tick_ = db_->PrepareStatement(query).as_nullable();
  }

  Ensures(insert_price_tick_ != nullptr);
  return *insert_price_tick_;
}
}  // namespace stonks::db