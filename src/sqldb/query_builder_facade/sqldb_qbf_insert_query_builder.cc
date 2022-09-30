#include "sqldb_qbf_insert_query_builder.h"

#include <gsl/assert>
#include <memory>
#include <utility>

#include "sqldb_qbf_common.h"

namespace stonks::sqldb::qbf {
InsertQueryBuilder::InsertQueryBuilder(cpp::NnSp<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto InsertQueryBuilder::WholeRow() -> InsertQueryBuilder& {
  Expects(!columns_.HasColumns());
  columns_.value.emplace<AllColumnsType>();
  Ensures(columns_.HasColumns());
  return *this;
}

auto InsertQueryBuilder::IntoTable(Table table) -> InsertQueryBuilder& {
  Expects(!table_.HasTable());
  table_ = std::move(table);
  Ensures(table_.HasTable());
  return *this;
}

auto InsertQueryBuilder::IntoTable(TableDefinition table_definition)
    -> InsertQueryBuilder& {
  Expects(!table_.HasTable());
  table_ = std::move(table_definition);
  Ensures(table_.HasTable());
  return *this;
}

auto InsertQueryBuilder::IntoColumns(std::vector<Column> columns)
    -> InsertQueryBuilder& {
  Expects(!columns_.HasColumns());
  columns_ = {std::move(columns)};
  Ensures(columns_.HasColumns());
  return *this;
}

auto InsertQueryBuilder::IntoColumns(
    const cpp::ConstView<ColumnDefinition>& column_definitions)
    -> InsertQueryBuilder& {
  return IntoColumns(GetColumns(column_definitions));
}

auto InsertQueryBuilder::Build() const -> Query {
  Expects(table_.HasTable());
  const auto& table = table_.GetTable();
  const auto columns_are_specified = columns_.HasColumns();
  const auto columns = columns_are_specified
                           ? GetColumns(table_, columns_)
                           : GetColumns(table_, {AllColumnsType{}});
  return query_builder_->BuildInsertQuery(table, columns);
}
}  // namespace stonks::sqldb::qbf