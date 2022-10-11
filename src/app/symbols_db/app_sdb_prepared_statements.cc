#include "app_sdb_prepared_statements.h"

#include <fmt/format.h>

#include <gsl/assert>

#include "app_sdb_tables.h"
#include "cpp_not_null.h"
#include "sqldb_qbf_delete_query_builder.h"
#include "sqldb_qbf_insert_query_builder.h"
#include "sqldb_qbf_select_query_builder.h"
#include "sqldb_query_builder_facade.h"
#include "sqldb_types.h"

namespace stonks::app::sdb {
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
}  // namespace stonks::app::sdb