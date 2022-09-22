#include "sqldb_qbf_select_query_builder.h"

#include <gsl/assert>
#include <memory>
#include <string>
#include <utility>
#include <variant>

#include "cpp_format.h"
#include "sqldb_qbf_common.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
SelectQueryBuilder::SelectQueryBuilder(cpp::NnSp<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto SelectQueryBuilder::Columns(std::vector<Column> columns)
    -> SelectQueryBuilder & {
  Expects(!columns_.HasColumns());
  columns_ = std::move(columns);
  Ensures(columns_.HasColumns());
  return *this;
}

auto SelectQueryBuilder::Columns(
    const cpp::ConstView<ColumnDefinition> &column_definitions)
    -> SelectQueryBuilder & {
  return Columns(GetColumns(column_definitions));
}

auto SelectQueryBuilder::AllColumns() -> SelectQueryBuilder & {
  Expects(!columns_.HasColumns());
  columns_.emplace<AllColumnsType>();
  Ensures(columns_.HasColumns());
  return *this;
}

auto SelectQueryBuilder::FromTable(Table table) -> SelectQueryBuilder & {
  Expects(!table_.has_value());
  table_ = std::move(table);
  Ensures(table_.has_value());
  return *this;
}

auto SelectQueryBuilder::FromTable(const TableDefinition &table_definition)
    -> SelectQueryBuilder & {
  Expects(!table_.has_value());
  table_ = table_definition.table;
  Ensures(table_.has_value());
  return *this;
}

auto SelectQueryBuilder::Where(std::string_view where_clause)
    -> SelectQueryBuilder & {
  Expects(!where_clause_.has_value());
  where_clause_ = cpp::Format("WHERE {}", where_clause);
  Ensures(where_clause_.has_value());
  return *this;
}

auto SelectQueryBuilder::And(std::string_view where_clause)
    -> SelectQueryBuilder & {
  Expects(where_clause_.has_value());
  (*where_clause_) += cpp::Format(" AND {}", where_clause);
  return *this;
}
auto SelectQueryBuilder::Or(std::string_view where_clause)
    -> SelectQueryBuilder & {
  Expects(where_clause_.has_value());
  (*where_clause_) += cpp::Format(" OR {}", where_clause);
  return *this;
}

auto SelectQueryBuilder::Limit(int limit) -> SelectQueryBuilder & {
  Expects(std::holds_alternative<std::monostate>(limit_));
  limit_.emplace<int>(limit);
  Ensures(!std::holds_alternative<std::monostate>(limit_));
  return *this;
}

auto SelectQueryBuilder::Limited() -> SelectQueryBuilder & {
  Expects(std::holds_alternative<std::monostate>(limit_));
  limit_.emplace<LimitedType>();
  Ensures(!std::holds_alternative<std::monostate>(limit_));
  return *this;
}

auto SelectQueryBuilder::Build() const -> Query {
  Expects(table_.has_value());
  Expects(columns_.HasColumns());
  const auto *const columns = columns_.GetColumns();
  return query_builder_->BuildSelectQuery(
      *table_, columns,
      where_clause_.value_or(std::string{}) + limit_.GetLimitClause());
}
}  // namespace stonks::sqldb::qbf