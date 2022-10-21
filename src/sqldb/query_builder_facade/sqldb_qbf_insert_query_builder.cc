#include "sqldb_qbf_insert_query_builder.h"

#include <gsl/assert>
#include <memory>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qbf_columns_variant.h"
#include "sqldb_qbf_common.h"

namespace stonks::sqldb::qbf {
InsertQueryBuilder::InsertQueryBuilder(cpp::NnUp<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto InsertQueryBuilder::WholeRow() -> InsertQueryBuilder& {
  Expects(!columns_.HasColumns());
  columns_.value.emplace<AllColumnsType>();
  Ensures(columns_.HasColumns());
  return *this;
}

auto InsertQueryBuilder::IntoTable(Table table) -> InsertQueryBuilder& {
  Expects(!table_.HasTable());
  table_.value = std::move(table);
  Ensures(table_.HasTable());
  return *this;
}

auto InsertQueryBuilder::IntoTable(TableDefinition table_definition)
    -> InsertQueryBuilder& {
  Expects(!table_.HasTable());
  table_.value = std::move(table_definition);
  Ensures(table_.HasTable());
  return *this;
}

auto InsertQueryBuilder::IntoColumns(std::vector<Column> columns)
    -> InsertQueryBuilder& {
  Expects(!columns_.HasColumns());
  columns_.value = std::move(columns);
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

auto InsertQueryBuilderTemplate::Build() const -> Query {
  Expects(!table_name_.empty());

  auto query = fmt::format("INSERT INTO {} ({}) VALUES ({})", table_name_,
                           column_names_, values_);

  Ensures(!query.empty());
  return {std::move(query)};
}
}  // namespace stonks::sqldb::qbf