#include "app_sdb_prepared_statements.h"

#include <fmt/core.h>
#include <fmt/format.h>

#include <function2/function2.hpp>
#include <gsl/assert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "app_sdb_tables.h"
#include "cpp_not_null.h"
#include "parametrized/sqldb_p_db.h"
#include "sqldb_qb_common.h"
#include "sqldb_query_builder.h"
#include "sqldb_types.h"
#include "sqldb_value.h"

namespace stonks::app::sdb {
namespace {
[[nodiscard]] auto SelectSymbolsInfo() {
  return sqldb::query_builder::Select<
             tables::SymbolInfo::name,
             tables::SymbolInfo::base_asset_min_amount,
             tables::SymbolInfo::base_asset_price_step,
             tables::SymbolInfo::quote_asset_min_amount,
             tables::SymbolInfo::quote_asset_price_step,
             sqldb::qb::As<BaseAsset::name, BaseAsset::base_asset>,
             sqldb::qb::As<QuoteAsset::name, QuoteAsset::quote_asset>>()
      .From<tables::SymbolInfo>()
      .Join<sqldb::qb::As<tables::Asset, BaseAsset>>(sqldb::qb::On(
          sqldb::qb::Column<tables::SymbolInfo::base_asset_id>() ==
          sqldb::qb::Column<BaseAsset::id>()))
      .Join<sqldb::qb::As<tables::Asset, QuoteAsset>>(sqldb::qb::On(
          sqldb::qb::Column<tables::SymbolInfo::quote_asset_id>() ==
          sqldb::qb::Column<QuoteAsset::id>()));
}
}  // namespace

auto PreparedStatementsFrom(const cpp::NnSp<sqldb::p::Db>& db)
    -> PreparedStatements {
  return {
      .create_asset{[db]() {
        return db->GetDb().PrepareStatement(
            sqldb::query_builder::CreateTable<tables::Asset>()
                .IfNotExists()
                .Build());
      }},

      .create_symbol_info{[db]() {
        return db->GetDb().PrepareStatement(
            sqldb::query_builder::CreateTable<tables::SymbolInfo>()
                .IfNotExists()
                .Build());
      }},

      .create_symbol_price_record{[db]() {
        return db->GetDb().PrepareStatement(
            sqldb::query_builder::CreateTable<tables::SymbolPriceRecord>()
                .IfNotExists()
                .Build());
      }},

      .select_assets{[db]() {
        return db->PrepareStatement(
            sqldb::query_builder::Select<tables::Asset::name>()
                .From<tables::Asset>()
                .Build());
      }},

      .insert_asset{[db]() {
        return db->PrepareStatement(
            sqldb::query_builder::Insert()
                .Value<tables::Asset::name>(
                    sqldb::qb::ParamForColumn<tables::Asset::name>())
                .Into<tables::Asset>()
                .Build());
      }},

      .delete_asset{[db]() {
        return db->PrepareStatement(
            sqldb::query_builder::DeleteFromTable<tables::Asset>()
                .Where(sqldb::qb::Column<tables::Asset::name>() ==
                       sqldb::qb::ParamForColumn<tables::Asset::name>())
                .Build());
      }},

      .select_symbols_with_price_records{[db]() {
        return db->PrepareStatement(
            sqldb::query_builder::Select<tables::SymbolInfo::name>()
                .From<tables::SymbolInfo>()
                .Where(sqldb::qb::Exists(
                    sqldb::query_builder::SelectOne()
                        .From<tables::SymbolPriceRecord>()
                        .Where(sqldb::qb::Column<
                                   tables::SymbolPriceRecord::symbol_id>() ==
                               sqldb::qb::Column<tables::SymbolInfo::id>())
                        .Limit(sqldb::Value{1})))
                .Build());
      }},

      .select_symbol_info{[db]() {
        return db->PrepareStatement(
            SelectSymbolsInfo()
                .Where(sqldb::qb::Column<tables::SymbolInfo::name>() ==
                       sqldb::qb::ParamForColumn<tables::SymbolInfo::name>())
                .Build());
      }},

      .select_symbols_info{
          [db]() { return db->PrepareStatement(SelectSymbolsInfo().Build()); }},

      .insert_symbol_info{[db]() {
        return db->PrepareStatement(
            sqldb::query_builder::Insert()
                .Value<tables::SymbolInfo::name>(
                    sqldb::qb::ParamForColumn<tables::SymbolInfo::name>())
                .Value<tables::SymbolInfo::base_asset_id>(
                    sqldb::query_builder::Select<tables::Asset::id>()
                        .From<tables::Asset>()
                        .Where(
                            sqldb::qb::Column<tables::Asset::name>() ==
                            sqldb::qb::ParamForColumn<tables::Asset::name>()))
                .Value<tables::SymbolInfo::base_asset_min_amount>(
                    sqldb::qb::ParamForColumn<
                        tables::SymbolInfo::base_asset_min_amount>())
                .Value<tables::SymbolInfo::base_asset_price_step>(
                    sqldb::qb::ParamForColumn<
                        tables::SymbolInfo::base_asset_price_step>())
                .Value<tables::SymbolInfo::quote_asset_id>(
                    sqldb::query_builder::Select<tables::Asset::id>()
                        .From<tables::Asset>()
                        .Where(
                            sqldb::qb::Column<tables::Asset::name>() ==
                            sqldb::qb::ParamForColumn<tables::Asset::name>()))
                .Value<tables::SymbolInfo::quote_asset_min_amount>(
                    sqldb::qb::ParamForColumn<
                        tables::SymbolInfo::quote_asset_min_amount>())
                .Value<tables::SymbolInfo::quote_asset_price_step>(
                    sqldb::qb::ParamForColumn<
                        tables::SymbolInfo::quote_asset_price_step>())
                .Into<tables::SymbolInfo>()
                .Build());
      }},

      .update_symbol_info{[db]() {
        return db->PrepareStatement(
            sqldb::query_builder::UpdateTable<tables::SymbolInfo>()
                .Set<tables::SymbolInfo::base_asset_id>(
                    sqldb::query_builder::Select<tables::Asset::id>()
                        .From<tables::Asset>()
                        .Where(
                            sqldb::qb::Column<tables::Asset::name>() ==
                            sqldb::qb::ParamForColumn<tables::Asset::name>()))
                .Set<tables::SymbolInfo::base_asset_min_amount>(
                    sqldb::qb::ParamForColumn<
                        tables::SymbolInfo::base_asset_min_amount>())
                .Set<tables::SymbolInfo::base_asset_price_step>(
                    sqldb::qb::ParamForColumn<
                        tables::SymbolInfo::base_asset_price_step>())
                .Set<tables::SymbolInfo::quote_asset_id>(
                    sqldb::query_builder::Select<tables::Asset::id>()
                        .From<tables::Asset>()
                        .Where(
                            sqldb::qb::Column<tables::Asset::name>() ==
                            sqldb::qb::ParamForColumn<tables::Asset::name>()))
                .Set<tables::SymbolInfo::quote_asset_min_amount>(
                    sqldb::qb::ParamForColumn<
                        tables::SymbolInfo::quote_asset_min_amount>())
                .Set<tables::SymbolInfo::quote_asset_price_step>(
                    sqldb::qb::ParamForColumn<
                        tables::SymbolInfo::quote_asset_price_step>())
                .Where(sqldb::qb::Column<tables::SymbolInfo::name>() ==
                       sqldb::qb::ParamForColumn<tables::SymbolInfo::name>())
                .Build());
      }},

      .delete_symbol_info{[db]() {
        return db->PrepareStatement(
            sqldb::query_builder::DeleteFromTable<tables::SymbolInfo>()
                .Where(sqldb::qb::Column<tables::SymbolInfo::name>() ==
                       sqldb::qb::ParamForColumn<tables::SymbolInfo::name>())
                .Build());
      }},

      .select_symbol_price_records{[db]() {
        return db->PrepareStatement(
            sqldb::query_builder::Select<tables::SymbolPriceRecord::price,
                                         tables::SymbolPriceRecord::time>()
                .From<tables::SymbolPriceRecord>()
                .Where(
                    (sqldb::qb::Column<
                         tables::SymbolPriceRecord::symbol_id>() ==
                     sqldb::query_builder::Select<tables::SymbolInfo::id>()
                         .From<tables::SymbolInfo>()
                         .Where(sqldb::qb::Column<tables::SymbolInfo::name>() ==
                                sqldb::qb::ParamForColumn<
                                    tables::SymbolInfo::name>())) &&
                    (sqldb::qb::Column<tables::SymbolPriceRecord::time>() >=
                     sqldb::qb::ParamForColumn<
                         tables::SymbolPriceRecord::time>()) &&
                    (sqldb::qb::Column<tables::SymbolPriceRecord::time>() <
                     sqldb::qb::ParamForColumn<
                         tables::SymbolPriceRecord::time>()))
                .Build());
      }},

      .insert_symbol_price_record{[db]() {
        return db->PrepareStatement(
            sqldb::query_builder::Insert()
                .Value<tables::SymbolPriceRecord::symbol_id>(
                    sqldb::query_builder::Select<tables::SymbolInfo::id>()
                        .From<tables::SymbolInfo>()
                        .Where(sqldb::qb::Column<tables::SymbolInfo::name>() ==
                               sqldb::qb::ParamForColumn<
                                   tables::SymbolInfo::name>()))
                .Value<tables::SymbolPriceRecord::price>(
                    sqldb::qb::ParamForColumn<
                        tables::SymbolPriceRecord::price>())
                .Value<tables::SymbolPriceRecord::time>(
                    sqldb::qb::ParamForColumn<
                        tables::SymbolPriceRecord::time>())
                .Into<tables::SymbolPriceRecord>()
                .Build());
      }},

      .delete_symbol_price_records{[db]() {
        return db->PrepareStatement(
            sqldb::query_builder::DeleteFromTable<tables::SymbolPriceRecord>()
                .Where(sqldb::qb::Column<tables::SymbolPriceRecord::time>() <
                       sqldb::qb::ParamForColumn<
                           tables::SymbolPriceRecord::time>())
                .Build());
      }}};
}
}  // namespace stonks::app::sdb