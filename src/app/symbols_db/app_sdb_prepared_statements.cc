#include "app_sdb_prepared_statements.h"

#include <fmt/core.h>
#include <fmt/format.h>

#include <gsl/assert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "app_sdb_tables.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "sqldb_qbf_delete_query_builder.h"
#include "sqldb_qbf_insert_query_builder.h"
#include "sqldb_qbf_select_query_builder.h"
#include "sqldb_query_builder_facade.h"
#include "sqldb_row_definition.h"
#include "sqldb_types.h"

namespace stonks::app::sdb {
// NOLINTNEXTLINE(*-unused-using-decls)
using fmt::literals::operator""_a;

namespace {
[[nodiscard]] auto SelectSymbolsInfoRowDefinition() {
  auto columns = tables::SymbolInfo::Definition().GetColumnDefinitions(
      {{tables::SymbolInfo::kName},
       {tables::SymbolInfo::kBaseAssetMinAmount},
       {tables::SymbolInfo::kBaseAssetPriceStep},
       {tables::SymbolInfo::kQuoteAssetMinAmount},
       {tables::SymbolInfo::kQuoteAssetPriceStep}});

  const auto base_asset_column = []() {
    auto base_asset_column =
        tables::Asset::Definition().GetColumnDefinition({tables::Asset::kName});
    base_asset_column.column.value = "base_asset";
    return base_asset_column;
  }();
  columns.emplace_back(cpp::AssumeNn(&base_asset_column));

  const auto quote_asset_column = []() {
    auto quote_asset_column =
        tables::Asset::Definition().GetColumnDefinition({tables::Asset::kName});
    quote_asset_column.column.value = "quote_asset";
    return quote_asset_column;
  }();
  columns.emplace_back(cpp::AssumeNn(&quote_asset_column));

  return sqldb::RowDefinition{columns};
}

[[nodiscard]] auto SelectSymbolsInfoQuery(
    const sqldb::RowDefinition& row_definition) {
  const auto& cell_definitions = row_definition.GetCellDefinitions();
  Expects(cell_definitions.size() == 7);

  return sqldb::Query{
      fmt::format("SELECT "
                  "{table0}.{column0}, "
                  "{table0}.{column1}, "
                  "{table0}.{column2}, "
                  "{table0}.{column3}, "
                  "{table0}.{column4}, "
                  "BaseAsset.{column7} AS {column5}, "
                  "QuoteAsset.{column7} AS {column6} "
                  "FROM {table0} "
                  "JOIN {table1} AS BaseAsset ON {table0}.{column8} = "
                  "BaseAsset.{column9} "
                  "JOIN {table1} AS QuoteAsset ON {table0}.{column10} = "
                  "QuoteAsset.{column9} ",
                  "table0"_a = tables::SymbolInfo::kTable,
                  "table1"_a = tables::Asset::kTable,
                  "column0"_a = cell_definitions[0].column.value,
                  "column1"_a = cell_definitions[1].column.value,
                  "column2"_a = cell_definitions[2].column.value,
                  "column3"_a = cell_definitions[3].column.value,
                  "column4"_a = cell_definitions[4].column.value,
                  "column5"_a = cell_definitions[5].column.value,
                  "column6"_a = cell_definitions[6].column.value,
                  "column7"_a = tables::Asset::kName,
                  "column8"_a = tables::SymbolInfo::kBaseAssetId,
                  "column9"_a = tables::Asset::kId,
                  "column10"_a = tables::SymbolInfo::kQuoteAssetId)};
}
}  // namespace

auto PreparedStatementsFrom(
    const cpp::NnSp<sqldb::IDb>& db,
    const sqldb::QueryBuilderFacade& query_builder_facade)
    -> PreparedStatements {
  return {
      .select_assets{[db, query_builder_facade]() {
        const auto& table = tables::Asset::Definition();
        const auto columns =
            table.GetColumnDefinitions({{tables::Asset::kName}});
        auto query = query_builder_facade.Select()
                         .Columns(columns)
                         .FromTable(table)
                         .Build();
        return db->PrepareStatement(std::move(query), columns);
      }},
      .insert_asset{[db, query_builder_facade]() {
        auto query = query_builder_facade.Insert()
                         .IntoTable(tables::Asset::Definition())
                         .IntoColumns({{tables::Asset::kName}})
                         .Build();
        return db->PrepareStatement(std::move(query));
      }},
      .delete_asset{[db, query_builder_facade]() {
        auto query = query_builder_facade.Delete()
                         .FromTable(tables::Asset::Definition())
                         .Where(fmt::format("{} = ?", tables::Asset::kName))
                         .Build();
        return db->PrepareStatement(std::move(query));
      }},
      .select_symbols_with_price_records{[db, query_builder_facade]() {
        const auto columns =
            tables::SymbolInfo::Definition().GetColumnDefinitions(
                {{tables::SymbolInfo::kName}});
        auto query = query_builder_facade.Select()
                         .Columns(columns)
                         .FromTable(tables::SymbolInfo::Definition())
                         .Where(fmt::format(
                             "EXISTS(SELECT 1 FROM {table0} WHERE "
                             "{table0}.{column0} = {table1}.{column1} LIMIT 1)",
                             "table0"_a = tables::SymbolPriceRecord::kTable,
                             "table1"_a = tables::SymbolInfo::kTable,
                             "column0"_a = tables::SymbolPriceRecord::kSymbolId,
                             "column1"_a = tables::SymbolInfo::kId))
                         .Build();
        return db->PrepareStatement(std::move(query), columns);
      }},
      .select_symbol_info{[db, query_builder_facade]() {
        const auto row_definition = SelectSymbolsInfoRowDefinition();
        auto query = sqldb::Query{fmt::format(
            "{} WHERE {}.{} = ?", SelectSymbolsInfoQuery(row_definition).value,
            tables::SymbolInfo::kTable, tables::SymbolInfo::kName)};
        return db->PrepareStatement(std::move(query), row_definition);
      }},
      .select_symbols_info{[db, query_builder_facade]() {
        const auto row_definition = SelectSymbolsInfoRowDefinition();
        auto query = SelectSymbolsInfoQuery(row_definition);
        return db->PrepareStatement(std::move(query), row_definition);
      }},
      .insert_symbol_info{[db, query_builder_facade]() {
        auto query = sqldb::Query{fmt::format(
            "INSERT INTO {table0} ({column0}, "
            "{column1}, {column2}, {column3}, "
            "{column4}, {column5}, {column6}) "
            "VALUES (?, "
            "(SELECT {column7} FROM {table1} WHERE {column8} = ?), ?, ?, "
            "(SELECT {column7} FROM {table1} WHERE {column8} = ?), ?, ?)",
            "table0"_a = tables::SymbolInfo::kTable,
            "table1"_a = tables::Asset::kTable,
            "column0"_a = tables::SymbolInfo::kName,
            "column1"_a = tables::SymbolInfo::kBaseAssetId,
            "column2"_a = tables::SymbolInfo::kBaseAssetMinAmount,
            "column3"_a = tables::SymbolInfo::kBaseAssetPriceStep,
            "column4"_a = tables::SymbolInfo::kQuoteAssetId,
            "column5"_a = tables::SymbolInfo::kQuoteAssetMinAmount,
            "column6"_a = tables::SymbolInfo::kQuoteAssetPriceStep,
            "column7"_a = tables::Asset::kId,
            "column8"_a = tables::Asset::kName)};
        return db->PrepareStatement(std::move(query));
      }},
      .update_symbol_info{[db, query_builder_facade]() {
        auto query = sqldb::Query{fmt::format(
            "UPDATE {table0} SET "
            "{column0} = (SELECT {column1} FROM {table1} WHERE {column2} = ?), "
            "{column3} = ?, "
            "{column4} = ?, "
            "{column5} = (SELECT {column1} FROM {table1} WHERE {column2} = ?), "
            "{column6} = ?, "
            "{column7} = ? "
            "WHERE {column8} = ?",
            "table0"_a = tables::SymbolInfo::kTable,
            "table1"_a = tables::Asset::kTable,
            "column0"_a = tables::SymbolInfo::kBaseAssetId,
            "column1"_a = tables::Asset::kId,
            "column2"_a = tables::Asset::kName,
            "column3"_a = tables::SymbolInfo::kBaseAssetMinAmount,
            "column4"_a = tables::SymbolInfo::kBaseAssetPriceStep,
            "column5"_a = tables::SymbolInfo::kQuoteAssetId,
            "column6"_a = tables::SymbolInfo::kQuoteAssetMinAmount,
            "column7"_a = tables::SymbolInfo::kQuoteAssetPriceStep,
            "column8"_a = tables::SymbolInfo::kName)};
        return db->PrepareStatement(std::move(query));
      }},
      .delete_symbol_info{[db, query_builder_facade]() {
        auto query =
            query_builder_facade.Delete()
                .FromTable(tables::SymbolInfo::Definition())
                .Where(fmt::format("{} = ?", tables::SymbolInfo::kName))
                .Build();
        return db->PrepareStatement(std::move(query));
      }},
      .select_symbol_price_records{[db, query_builder_facade]() {
        const auto& table = tables::SymbolPriceRecord::Definition();
        const auto columns =
            table.GetColumnDefinitions({{tables::SymbolPriceRecord::kPrice},
                                        {tables::SymbolPriceRecord::kTime}});
        auto query =
            query_builder_facade.Select()
                .Columns(columns)
                .FromTable(table)
                .Where(fmt::format(
                    "{column0} = "
                    "(SELECT {column1} FROM {table} WHERE {column2} = ?) AND "
                    "{column3} >= ? AND {column3} < ?",
                    "table"_a = tables::SymbolInfo::kTable,
                    "column0"_a = tables::SymbolPriceRecord::kSymbolId,
                    "column1"_a = tables::SymbolInfo::kId,
                    "column2"_a = tables::SymbolInfo::kName,
                    "column3"_a = tables::SymbolPriceRecord::kTime))
                .Limited()
                .Build();
        return db->PrepareStatement(std::move(query), columns);
      }},
      .insert_symbol_price_record{[db, query_builder_facade]() {
        auto query = sqldb::Query{fmt::format(
            "INSERT INTO {table0} "
            "({column0}, {column1}, {column2}) "
            "VALUES "
            "((SELECT {column3} FROM {table1} WHERE {column4} = ?), ?, ?)",
            "table0"_a = tables::SymbolPriceRecord::kTable,
            "table1"_a = tables::SymbolInfo::kTable,
            "column0"_a = tables::SymbolPriceRecord::kSymbolId,
            "column1"_a = tables::SymbolPriceRecord::kPrice,
            "column2"_a = tables::SymbolPriceRecord::kTime,
            "column3"_a = tables::SymbolInfo::kId,
            "column4"_a = tables::SymbolInfo::kName)};
        return db->PrepareStatement(std::move(query));
      }},
      .delete_symbol_price_records{[db, query_builder_facade]() {
        auto query =
            query_builder_facade.Delete()
                .FromTable(tables::SymbolPriceRecord::Definition())
                .Where(fmt::format("{} < ?", tables::SymbolPriceRecord::kTime))
                .Build();
        return db->PrepareStatement(std::move(query));
      }}};
}
}  // namespace stonks::app::sdb