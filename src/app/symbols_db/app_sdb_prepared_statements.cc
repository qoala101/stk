#include "app_sdb_prepared_statements.h"

#include <fmt/core.h>
#include <fmt/format.h>

#include <gsl/assert>
#include <memory>
#include <string>
#include <utility>

#include "app_sdb_tables.h"
#include "cpp_not_null.h"
#include "sqldb_qbf_delete_query_builder.h"
#include "sqldb_qbf_insert_query_builder.h"
#include "sqldb_qbf_select_query_builder.h"
#include "sqldb_query_builder_facade.h"
#include "sqldb_types.h"

namespace stonks::app::sdb {
// NOLINTNEXTLINE(*-unused-using-decls)
using fmt::literals::operator""_a;

namespace {
template <typename Statement>
[[nodiscard]] auto InitLazily(
    cpp::Up<Statement>& statement,
    const cpp::InvocableReturning<cpp::NnUp<Statement>> auto& initializer)
    -> auto& {
  if (statement == nullptr) {
    statement = initializer().as_nullable();
  }

  Ensures(statement != nullptr);
  return *statement;
}
}  // namespace

PreparedStatements::PreparedStatements(
    cpp::NnSp<sqldb::IDb> db, sqldb::QueryBuilderFacade query_builder_facade)
    : db_{std::move(db)},
      query_builder_facade_{std::move(query_builder_facade)} {}

auto PreparedStatements::SelectAssets() const
    -> const sqldb::ISelectStatement& {
  return InitLazily(select_assets_, [this]() {
    const auto& table = tables::Asset::Definition();
    const auto columns = table.GetColumnDefinitions({{tables::Asset::kName}});
    auto query = query_builder_facade_.Select()
                     .Columns(columns)
                     .FromTable(table)
                     .Build();
    return db_->PrepareStatement(std::move(query), columns);
  });
}

auto PreparedStatements::InsertAsset() const -> const sqldb::IUpdateStatement& {
  return InitLazily(insert_asset_, [this]() {
    auto query = query_builder_facade_.Insert()
                     .IntoTable(tables::Asset::Definition())
                     .IntoColumns({{tables::Asset::kName}})
                     .Build();
    return db_->PrepareStatement(std::move(query));
  });
}

auto PreparedStatements::DeleteAsset() const -> const sqldb::IUpdateStatement& {
  return InitLazily(delete_asset_, [this]() {
    auto query = query_builder_facade_.Delete()
                     .FromTable(tables::Asset::Definition())
                     .Where(fmt::format("{}.{} = ?", tables::Asset::kTable,
                                        tables::Asset::kName))
                     .Build();
    return db_->PrepareStatement(std::move(query));
  });
}

auto PreparedStatements::SelectSymbolPriceRecords() const
    -> const sqldb::ISelectStatement& {
  return InitLazily(select_symbol_price_records_, [this]() {
    const auto& table = tables::SymbolPriceRecord::Definition();
    const auto columns =
        table.GetColumnDefinitions({{tables::SymbolPriceRecord::kPrice},
                                    {tables::SymbolPriceRecord::kTime}});
    auto query = query_builder_facade_.Select()
                     .Columns(columns)
                     .FromTable(table)
                     .Where(fmt::format(
                         "{table0}.{column0} = "
                         "(SELECT id FROM {table1} WHERE name = ?) AND "
                         "{table0}.{column1} >= ? AND "
                         "{table0}.{column1} <= ?",
                         "table0"_a = tables::SymbolPriceRecord::kTable,
                         "table1"_a = tables::SymbolInfo::kTable,
                         "column0"_a = tables::SymbolPriceRecord::kSymbolId,
                         "column1"_a = tables::SymbolPriceRecord::kTime))
                     .Limited()
                     .Build();
    return db_->PrepareStatement(std::move(query), columns);
  });
}

auto PreparedStatements::InsertSymbolPriceRecord() const
    -> const sqldb::IUpdateStatement& {
  return InitLazily(insert_symbol_price_record_, [this]() {
    auto query = sqldb::Query{
        fmt::format("INSERT INTO {table0} "
                    "({column0}, {column1}, {column2}) "
                    "VALUES "
                    "((SELECT id FROM {table1} WHERE name = ?), ?, ?)",
                    "table0"_a = tables::SymbolPriceRecord::kTable,
                    "table1"_a = tables::SymbolInfo::kTable,
                    "column0"_a = tables::SymbolPriceRecord::kSymbolId,
                    "column1"_a = tables::SymbolPriceRecord::kPrice,
                    "column2"_a = tables::SymbolPriceRecord::kTime)};
    return db_->PrepareStatement(std::move(query));
  });
}

auto PreparedStatements::DeleteSymbolPriceRecords() const
    -> const sqldb::IUpdateStatement& {
  return InitLazily(insert_symbol_price_record_, [this]() {
    auto query =
        query_builder_facade_.Delete()
            .FromTable(tables::SymbolPriceRecord::Definition())
            .Where(fmt::format("{}.{} < ?", tables::SymbolPriceRecord::kTable,
                               tables::SymbolPriceRecord::kTime))
            .Build();
    return db_->PrepareStatement(std::move(query));
  });
}
}  // namespace stonks::app::sdb