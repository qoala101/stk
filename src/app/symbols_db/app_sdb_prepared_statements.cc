#include "app_sdb_prepared_statements.h"

#include <fmt/core.h>
#include <fmt/format.h>
#include <sql.h>

#include <function2/function2.hpp>
#include <gsl/assert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "app_sdb_tables.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "sqldb_row_definition.h"
#include "sqldb_types.h"

namespace stonks::app::sdb {
namespace {
[[nodiscard]] auto SymbolsInfoBaseAssetColumn() -> auto& {
  static const auto kColumn = []() {
    auto base_asset_column =
        tables::Asset::Definition().GetColumnDefinition({tables::Asset::kName});
    base_asset_column.column.value = "base_asset";
    return base_asset_column;
  }();

  return kColumn;
}

[[nodiscard]] auto SymbolsInfoQuoteAssetColumn() -> auto& {
  static const auto kColumn = []() {
    auto quote_asset_column =
        tables::Asset::Definition().GetColumnDefinition({tables::Asset::kName});
    quote_asset_column.column.value = "quote_asset";
    return quote_asset_column;
  }();

  return kColumn;
}

[[nodiscard]] auto SelectSymbolsInfoRowDefinition() -> auto& {
  static const auto kDefinition = sqldb::RowDefinition{[]() {
    auto columns = tables::SymbolInfo::Definition().GetColumnDefinitions(
        {{tables::SymbolInfo::kName},
         {tables::SymbolInfo::kBaseAssetMinAmount},
         {tables::SymbolInfo::kBaseAssetPriceStep},
         {tables::SymbolInfo::kQuoteAssetMinAmount},
         {tables::SymbolInfo::kQuoteAssetPriceStep}});
    columns.emplace_back(cpp::AssumeNn(&SymbolsInfoBaseAssetColumn()));
    columns.emplace_back(cpp::AssumeNn(&SymbolsInfoQuoteAssetColumn()));
    return columns;
  }()};

  return kDefinition;
}

[[nodiscard]] auto SelectSymbolsInfoQuery(sql::SelectModel& select_model)
    -> auto& {
  static const auto& kCellDefinitions =
      SelectSymbolsInfoRowDefinition().GetCellDefinitions();
  Expects(kCellDefinitions.size() == 7);

  static constexpr auto kBaseAssetTable = "BaseAsset";
  static constexpr auto kQuoteAssetTable = "QuoteAsset";
  static constexpr auto kJoin0 = "Join0";

  select_model
      .select(
          fmt::format("{}.{}", kJoin0, kCellDefinitions[0].column.value),
          kCellDefinitions[1].column.value, kCellDefinitions[2].column.value,
          kCellDefinitions[3].column.value, kCellDefinitions[4].column.value,
          kCellDefinitions[5].column.value,
          fmt::format("{}.{} as {}", kQuoteAssetTable, tables::Asset::kName,
                      kCellDefinitions[6].column.value))
      .from(fmt::format(
          "({}) as {}",
          sql::SelectModel{}
              .select("*", fmt::format("{}.{} as {}", kBaseAssetTable,
                                       tables::Asset::kName,
                                       kCellDefinitions[5].column.value))
              .from(tables::SymbolInfo::kTable)
              .join(fmt::format("{} as {}", tables::Asset::kTable,
                                kBaseAssetTable))
              .on(sql::column{tables::SymbolInfo::kBaseAssetId} ==
                  sql::column{fmt::format("{}.{}", kBaseAssetTable,
                                          tables::Asset::kId)})
              .str(),
          kJoin0))
      .join(fmt::format("{} as {}", tables::Asset::kTable, kQuoteAssetTable))
      .on(sql::column{tables::SymbolInfo::kQuoteAssetId} ==
          sql::column{
              fmt::format("{}.{}", kQuoteAssetTable, tables::Asset::kId)});

  return select_model;
}
}  // namespace

auto PreparedStatementsFrom(const cpp::NnSp<sqldb::IDb>& db)
    -> PreparedStatements {
  return {
      .select_assets{[db]() {
        static const auto kQuery =
            sqldb::Query{sql::SelectModel{}
                             .select(tables::Asset::kName)
                             .from(tables::Asset::kTable)
                             .str()};
        static const auto kColumns =
            tables::Asset::Definition().GetColumnDefinitions(
                {{tables::Asset::kName}});
        return db->PrepareStatement(kQuery, kColumns);
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
        static const auto kQuery = sqldb::Query{
            sql::SelectModel{}
                .select(tables::SymbolInfo::kName)
                .from(tables::SymbolInfo::kTable)
                .where(fmt::format(
                    "exists({})",
                    sql::SelectModel{}
                        .select("1")
                        .from(tables::SymbolPriceRecord::kTable)
                        .where(sql::column{fmt::format(
                                   "{}.{}", tables::SymbolPriceRecord::kTable,
                                   tables::SymbolPriceRecord::kSymbolId)} ==
                               sql::column{fmt::format(
                                   "{}.{}", tables::SymbolInfo::kTable,
                                   tables::SymbolInfo::kId)})
                        .limit(1)
                        .str()))
                .str()};
        static const auto kColumns =
            tables::SymbolInfo::Definition().GetColumnDefinitions(
                {{tables::SymbolInfo::kName}});
        return db->PrepareStatement(kQuery, kColumns);
      }},

      .select_symbol_info{[db]() {
        static const auto kQuery = []() {
          auto select_model = sql::SelectModel{};
          auto query = sqldb::Query{
              SelectSymbolsInfoQuery(select_model)
                  .where(sql::column{fmt::format(
                             "{}.{}", tables::SymbolInfo::kTable,
                             tables::SymbolInfo::kName)} == sql::Param{"?"})
                  .str()};
          return query;
        }();
        return db->PrepareStatement(kQuery, SelectSymbolsInfoRowDefinition());
      }},

      .select_symbols_info{[db]() {
        static const auto kQuery = []() {
          auto select_model = sql::SelectModel{};
          auto query = sqldb::Query{SelectSymbolsInfoQuery(select_model).str()};
          return query;
        }();
        return db->PrepareStatement(kQuery, SelectSymbolsInfoRowDefinition());
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
        static const auto kQuery = sqldb::Query{
            sql::SelectModel{}
                .select(tables::SymbolPriceRecord::kPrice,
                        tables::SymbolPriceRecord::kTime)
                .from(tables::SymbolPriceRecord::kTable)
                .where((sql::column{tables::SymbolPriceRecord::kSymbolId} ==
                        sql::Param{fmt::format(
                            "({})",
                            sql::SelectModel{}
                                .select(tables::SymbolInfo::kId)
                                .from(tables::SymbolInfo::kTable)
                                .where(sql::column{tables::SymbolInfo::kName} ==
                                       sql::Param{"?"})
                                .str())}) &&
                       (sql::column{tables::SymbolPriceRecord::kTime} >=
                        sql::Param{"?"}) &&
                       (sql::column{tables::SymbolPriceRecord::kTime} <
                        sql::Param{"?"}))
                .str()};
        static const auto kColumns =
            tables::SymbolPriceRecord::Definition().GetColumnDefinitions(
                {{tables::SymbolPriceRecord::kPrice},
                 {tables::SymbolPriceRecord::kTime}});
        return db->PrepareStatement(kQuery, kColumns);
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