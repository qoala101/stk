#include "app_sdb_prepared_statements.h"

#include <fmt/core.h>
#include <fmt/format.h>
#include <sql.h>

#include <function2/function2.hpp>
#include <gsl/assert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "app_sdb_tables.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "di_factory.h"
#include "sqldb_query_builder_facade.h"
#include "sqldb_row_definition.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"
#include "sqlite_query_builder.h"

namespace stonks::app::sdb {
namespace {
[[nodiscard]] auto SelectSymbolsInfoQuery() {
  struct BaseAsset {
    struct id : tables::Asset::id {
      using Table = BaseAsset;
    };

    struct name : tables::Asset::name {
      using Table = BaseAsset;
    };

    struct base_asset : name {};
  };

  struct QuoteAsset {
    struct id : tables::Asset::id {
      using Table = QuoteAsset;
    };

    struct name : tables::Asset::name {
      using Table = QuoteAsset;
    };

    struct quote_asset : name {};
  };

  return sqldb::QueryBuilderFacadeT{}
      .Select<tables::SymbolInfo::name,
              tables::SymbolInfo::base_asset_min_amount,
              tables::SymbolInfo::base_asset_price_step,
              tables::SymbolInfo::quote_asset_min_amount,
              tables::SymbolInfo::quote_asset_price_step,
              sqldb::As<BaseAsset::name, BaseAsset::base_asset>,
              sqldb::As<QuoteAsset::name, QuoteAsset::quote_asset>>()
      .From<tables::SymbolInfo>()
      .Join<sqldb::As<tables::Asset, BaseAsset>>(sqldb::qbf::On(
          sqldb::qbf::Column<tables::SymbolInfo::base_asset_id>{} ==
          sqldb::qbf::Column<BaseAsset::id>{}))
      .Join<sqldb::As<tables::Asset, QuoteAsset>>(sqldb::qbf::On(
          sqldb::qbf::Column<tables::SymbolInfo::quote_asset_id>{} ==
          sqldb::qbf::Column<QuoteAsset::id>{}));
}
}  // namespace

auto PreparedStatementsFrom(const cpp::NnSp<sqldb::IDb>& db)
    -> PreparedStatements {
  static auto qbf = sqldb::QueryBuilderFacadeT{};

  return {
      .select_assets{[db]() {
        auto [query, columns] =
            qbf.Select<tables::Asset::name>().From<tables::Asset>().Build();
        return db->PrepareStatement(std::move(query), std::move(columns));
      }},

      .insert_asset{[db]() {
        auto query = qbf.Insert()
                         .Value<tables::Asset::name>(sqldb::qbf::Param{}.text_)
                         .Into<tables::Asset>()
                         .Build();
        return db->PrepareStatement(std::move(query));
      }},

      .delete_asset{[db]() {
        auto query = qbf.Delete()
                         .From<tables::Asset>()
                         .Where(sqldb::qbf::Column<tables::Asset::name>{} ==
                                sqldb::qbf::Param{})
                         .Build();
        return db->PrepareStatement(std::move(query));
      }},

      .select_symbols_with_price_records{[db]() {
        auto [query, columns] =
            qbf.Select<tables::SymbolInfo::name>()
                .From<tables::SymbolInfo>()
                .Where(sqldb::qbf::Exists(
                    qbf.SelectOne()
                        .From<tables::SymbolPriceRecord>()
                        .Where(sqldb::qbf::Column<
                                   tables::SymbolPriceRecord::symbol_id>{} ==
                               sqldb::qbf::Column<tables::SymbolInfo::id>{})
                        .Limit(1)))
                .Build();
        return db->PrepareStatement(std::move(query), std::move(columns));
      }},

      .select_symbol_info{[db]() {
        auto [query, columns] =
            SelectSymbolsInfoQuery()
                .Where(sqldb::qbf::Column<tables::SymbolInfo::name>{} ==
                       sqldb::qbf::Param{})
                .Build();
        return db->PrepareStatement(std::move(query), std::move(columns));
      }},

      .select_symbols_info{[db]() {
        auto [query, columns] = SelectSymbolsInfoQuery().Build();
        return db->PrepareStatement(std::move(query), std::move(columns));
      }},

      .insert_symbol_info{[db]() {
        auto query =
            qbf.Insert()
                .Value<tables::SymbolInfo::name>(sqldb::qbf::Param{}.text_)
                .Value<tables::SymbolInfo::base_asset_id>(sqldb::qbf::Param{
                    qbf.Select<tables::Asset::id>().From<tables::Asset>().Where(
                        sqldb::qbf::Column<tables::Asset::name>{} ==
                        sqldb::qbf::Param{})}
                                                              .text_)
                .Value<tables::SymbolInfo::base_asset_min_amount>(
                    sqldb::qbf::Param{}.text_)
                .Value<tables::SymbolInfo::base_asset_price_step>(
                    sqldb::qbf::Param{}.text_)
                .Value<tables::SymbolInfo::quote_asset_id>(sqldb::qbf::Param{
                    qbf.Select<tables::Asset::id>().From<tables::Asset>().Where(
                        sqldb::qbf::Column<tables::Asset::name>{} ==
                        sqldb::qbf::Param{})}
                                                               .text_)
                .Value<tables::SymbolInfo::quote_asset_min_amount>(
                    sqldb::qbf::Param{}.text_)
                .Value<tables::SymbolInfo::quote_asset_price_step>(
                    sqldb::qbf::Param{}.text_)
                .Into<tables::SymbolInfo>()
                .Build();
        return db->PrepareStatement(std::move(query));
      }},

      .update_symbol_info{[db]() {
        auto query =
            qbf.Update<tables::SymbolInfo>()
                .Set<tables::SymbolInfo::base_asset_id>(sqldb::qbf::Param{
                    qbf.Select<tables::Asset::id>().From<tables::Asset>().Where(
                        sqldb::qbf::Column<tables::Asset::name>{} ==
                        sqldb::qbf::Param{})}
                                                            .text_)
                .Set<tables::SymbolInfo::base_asset_min_amount>(
                    sqldb::qbf::Param{}.text_)
                .Set<tables::SymbolInfo::base_asset_price_step>(
                    sqldb::qbf::Param{}.text_)
                .Set<tables::SymbolInfo::quote_asset_id>(sqldb::qbf::Param{
                    qbf.Select<tables::Asset::id>().From<tables::Asset>().Where(
                        sqldb::qbf::Column<tables::Asset::name>{} ==
                        sqldb::qbf::Param{})}
                                                             .text_)
                .Set<tables::SymbolInfo::quote_asset_min_amount>(
                    sqldb::qbf::Param{}.text_)
                .Set<tables::SymbolInfo::quote_asset_price_step>(
                    sqldb::qbf::Param{}.text_)
                .Where(sqldb::qbf::Column<tables::SymbolInfo::name>{} ==
                       sqldb::qbf::Param{})
                .Build();
        return db->PrepareStatement(std::move(query));
      }},

      .delete_symbol_info{[db]() {
        auto query =
            qbf.Delete()
                .From<tables::SymbolInfo>()
                .Where(sqldb::qbf::Column<tables::SymbolInfo::name>{} ==
                       sqldb::qbf::Param{})
                .Build();
        return db->PrepareStatement(std::move(query));
      }},

      .select_symbol_price_records{[db]() {
        auto [query, columns] =
            qbf.Select<tables::SymbolPriceRecord::price,
                       tables::SymbolPriceRecord::time>()
                .From<tables::SymbolPriceRecord>()
                .Where(sqldb::qbf::Column<
                           tables::SymbolPriceRecord::symbol_id>{} ==
                       sqldb::qbf::Param{
                           qbf.Select<tables::SymbolInfo::id>()
                               .From<tables::SymbolInfo>()
                               .Where(sqldb::qbf::Column<
                                          tables::SymbolInfo::name>{} ==
                                      sqldb::qbf::Param{})})
                .And(sqldb::qbf::Column<tables::SymbolPriceRecord::time>{} >=
                     sqldb::qbf::Param{})
                .And(sqldb::qbf::Column<tables::SymbolPriceRecord::time>{} <
                     sqldb::qbf::Param{})
                .Build();
        return db->PrepareStatement(std::move(query), std::move(columns));
      }},

      .insert_symbol_price_record{[db]() {
        auto query =
            qbf.Insert()
                .Value<tables::SymbolPriceRecord::symbol_id>(sqldb::qbf::Param{
                    qbf.Select<tables::SymbolInfo::id>()
                        .From<tables::SymbolInfo>()
                        .Where(sqldb::qbf::Column<tables::SymbolInfo::name>{} ==
                               sqldb::qbf::Param{})}
                                                                 .text_)
                .Value<tables::SymbolPriceRecord::price>(
                    sqldb::qbf::Param{}.text_)
                .Value<tables::SymbolPriceRecord::time>(
                    sqldb::qbf::Param{}.text_)
                .Into<tables::SymbolPriceRecord>()
                .Build();
        return db->PrepareStatement(std::move(query));
      }},

      .delete_symbol_price_records{[db]() {
        auto query =
            qbf.Delete()
                .From<tables::SymbolPriceRecord>()
                .Where(sqldb::qbf::Column<tables::SymbolPriceRecord::time>{} <
                       sqldb::qbf::Param{})
                .Build();
        return db->PrepareStatement(std::move(query));
      }}};
}
}  // namespace stonks::app::sdb