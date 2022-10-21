#include "sqldb_qbf_update_query_builder.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qbf_common.h"

namespace stonks::sqldb::qbf {
UpdateQueryBuilder::UpdateQueryBuilder(cpp::NnUp<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto UpdateQueryBuilder::OfTable(Table table) -> UpdateQueryBuilder& {
  Expects(!table_.HasTable());
  table_.value = std::move(table);
  Ensures(table_.HasTable());
  return *this;
}

auto UpdateQueryBuilder::OfTable(TableDefinition table_definition)
    -> UpdateQueryBuilder& {
  Expects(!table_.HasTable());
  table_.value = std::move(table_definition);
  Ensures(table_.HasTable());
  return *this;
}

auto UpdateQueryBuilder::Columns(std::vector<Column> columns)
    -> UpdateQueryBuilder& {
  Expects(!columns_.HasColumns());
  columns_.value = std::move(columns);
  Ensures(columns_.HasColumns());
  return *this;
}

auto UpdateQueryBuilder::Columns(
    const cpp::ConstView<ColumnDefinition>& column_definitions)
    -> UpdateQueryBuilder& {
  return Columns(GetColumns(column_definitions));
}

auto UpdateQueryBuilder::AllColumns() -> UpdateQueryBuilder& {
  Expects(!columns_.HasColumns());
  columns_.value.emplace<AllColumnsType>();
  Ensures(columns_.HasColumns());
  return *this;
}

auto UpdateQueryBuilder::Where(std::string_view where_clause)
    -> UpdateQueryBuilder& {
  Expects(!where_clause_.has_value());
  where_clause_ = fmt::format("WHERE {}", where_clause);
  Ensures(where_clause_.has_value());
  return *this;
}

auto UpdateQueryBuilder::Build() const -> Query {
  Expects(table_.HasTable());
  const auto& table = table_.GetTable();
  const auto columns = GetColumns(table_, columns_);
  return query_builder_->BuildUpdateQuery(
      table, columns, where_clause_.value_or(std::string{}));
}

auto UpdateQueryBuilderTemplate::Where(std::string_view where_clause)
    -> UpdateQueryBuilderTemplate& {
  Expects(where_clause_.empty());
  where_clause_ = fmt::format(" WHERE ({})", where_clause);
  Ensures(!where_clause_.empty());
  return *this;
}

auto UpdateQueryBuilderTemplate::And(std::string_view where_clause)
    -> UpdateQueryBuilderTemplate& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" AND ({})", where_clause);
  return *this;
}
auto UpdateQueryBuilderTemplate::Or(std::string_view where_clause)
    -> UpdateQueryBuilderTemplate& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" OR ({})", where_clause);
  return *this;
}

auto UpdateQueryBuilderTemplate::Build() const -> Query {
  Expects(!table_name_.empty());
  Expects(!column_values_.empty());
  auto query = fmt::format("UPDATE {} SET {}{}", table_name_, column_values_,
                           where_clause_);
  return {std::move(query)};
}
}  // namespace stonks::sqldb::qbf