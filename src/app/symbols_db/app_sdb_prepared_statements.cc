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
        static const auto kQuery =
            sqldb::Query{sql::InsertModel{}
                             .insert(tables::Asset::kName, sql::Param{"?"})
                             .into(tables::Asset::kTable)
                             .str()};
        return db->PrepareStatement(kQuery);
      }},

      .delete_asset{[db]() {
        static const auto kQuery = sqldb::Query{
            sql::DeleteModel{}
                ._delete()
                .from(tables::Asset::kTable)
                .where(sql::column{tables::Asset::kName} == sql::Param{"?"})
                .str()};
        return db->PrepareStatement(kQuery);
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
        static const auto kQuery = sqldb::Query{
            sql::InsertModel{}
                .insert(tables::SymbolInfo::kName, sql::Param{"?"})(
                    tables::SymbolInfo::kBaseAssetId,
                    sql::Param{fmt::format(
                        "({})", sql::SelectModel{}
                                    .select(tables::Asset::kId)
                                    .from(tables::Asset::kTable)
                                    .where(sql::column{tables::Asset::kName} ==
                                           sql::Param{"?"})
                                    .str())})(
                    tables::SymbolInfo::kBaseAssetMinAmount, sql::Param{"?"})(
                    tables::SymbolInfo::kBaseAssetPriceStep, sql::Param{"?"})(
                    tables::SymbolInfo::kQuoteAssetId,
                    sql::Param{fmt::format(
                        "({})", sql::SelectModel{}
                                    .select(tables::Asset::kId)
                                    .from(tables::Asset::kTable)
                                    .where(sql::column{tables::Asset::kName} ==
                                           sql::Param{"?"})
                                    .str())})(
                    tables::SymbolInfo::kQuoteAssetMinAmount, sql::Param{"?"})(
                    tables::SymbolInfo::kQuoteAssetPriceStep, sql::Param{"?"})
                .into(tables::SymbolInfo::kTable)
                .str()};
        return db->PrepareStatement(kQuery);
      }},

      .update_symbol_info{[db]() {
        static const auto kQuery = sqldb::Query{
            sql::UpdateModel{}
                .update(tables::SymbolInfo::kTable)
                .set(tables::SymbolInfo::kBaseAssetId,
                     sql::Param{fmt::format(
                         "({})", sql::SelectModel{}
                                     .select(tables::Asset::kId)
                                     .from(tables::Asset::kTable)
                                     .where(sql::column{tables::Asset::kName} ==
                                            sql::Param{"?"})
                                     .str())})(
                    tables::SymbolInfo::kBaseAssetMinAmount, sql::Param{"?"})(
                    tables::SymbolInfo::kBaseAssetPriceStep, sql::Param{"?"})(
                    tables::SymbolInfo::kQuoteAssetId,
                    sql::Param{fmt::format(
                        "({})", sql::SelectModel{}
                                    .select(tables::Asset::kId)
                                    .from(tables::Asset::kTable)
                                    .where(sql::column{tables::Asset::kName} ==
                                           sql::Param{"?"})
                                    .str())})(
                    tables::SymbolInfo::kQuoteAssetMinAmount, sql::Param{"?"})(
                    tables::SymbolInfo::kQuoteAssetPriceStep, sql::Param{"?"})
                .where(sql::column{tables::SymbolInfo::kName} ==
                       sql::Param{"?"})
                .str()};
        return db->PrepareStatement(kQuery);
      }},

      .delete_symbol_info{[db]() {
        static const auto kQuery =
            sqldb::Query{sql::DeleteModel{}
                             ._delete()
                             .from(tables::SymbolInfo::kTable)
                             .where(sql::column{tables::SymbolInfo::kName} ==
                                    sql::Param{"?"})
                             .str()};
        return db->PrepareStatement(kQuery);
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
        static const auto kQuery = sqldb::Query{
            sql::InsertModel{}
                .insert(tables::SymbolPriceRecord::kSymbolId,
                        sql::Param{fmt::format(
                            "({})",
                            sql::SelectModel{}
                                .select(tables::SymbolInfo::kId)
                                .from(tables::SymbolInfo::kTable)
                                .where(sql::column{tables::SymbolInfo::kName} ==
                                       sql::Param{"?"})
                                .str())})(tables::SymbolPriceRecord::kPrice,
                                          sql::Param{"?"})(
                    tables::SymbolPriceRecord::kTime, sql::Param{"?"})
                .into(tables::SymbolPriceRecord::kTable)
                .str()};
        return db->PrepareStatement(kQuery);
      }},

      .delete_symbol_price_records{[db]() {
        static const auto kQuery = sqldb::Query{
            sql::DeleteModel{}
                ._delete()
                .from(tables::SymbolPriceRecord::kTable)
                .where(sql::column{tables::SymbolPriceRecord::kTime} <
                       sql::Param{"?"})
                .str()};
        return db->PrepareStatement(kQuery);
      }}};
}
}  // namespace stonks::app::sdb