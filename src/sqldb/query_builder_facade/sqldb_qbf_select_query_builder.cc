#include "sqldb_qbf_select_query_builder.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <iostream>
#include <memory>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qbf_common.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
SelectQueryBuilder::SelectQueryBuilder(cpp::NnUp<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto SelectQueryBuilder::Columns(std::vector<Column> columns)
    -> SelectQueryBuilder & {
  Expects(!columns_.HasColumns());
  columns_.value = std::move(columns);
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
  columns_.value.emplace<AllColumnsType>();
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
  where_clause_ = fmt::format("WHERE {}", where_clause);
  Ensures(where_clause_.has_value());
  return *this;
}

auto SelectQueryBuilder::And(std::string_view where_clause)
    -> SelectQueryBuilder & {
  Expects(where_clause_.has_value());
  (*where_clause_) += fmt::format(" AND {}", where_clause);
  return *this;
}
auto SelectQueryBuilder::Or(std::string_view where_clause)
    -> SelectQueryBuilder & {
  Expects(where_clause_.has_value());
  (*where_clause_) += fmt::format(" OR {}", where_clause);
  return *this;
}

auto SelectQueryBuilder::Limit(int limit) -> SelectQueryBuilder & {
  Expects(std::holds_alternative<std::monostate>(limit_.value));
  limit_.value.emplace<int>(limit);
  Ensures(!std::holds_alternative<std::monostate>(limit_.value));
  return *this;
}

auto SelectQueryBuilder::Limited() -> SelectQueryBuilder & {
  Expects(std::holds_alternative<std::monostate>(limit_.value));
  limit_.value.emplace<LimitedType>();
  Ensures(!std::holds_alternative<std::monostate>(limit_.value));
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

auto SelectQueryBuilderTemplate::Where(std::string_view where_clause)
    -> SelectQueryBuilderTemplate & {
  Expects(where_clause_.empty());
  where_clause_ = fmt::format(" WHERE ({})", where_clause);
  Ensures(!where_clause_.empty());
  return *this;
}

auto SelectQueryBuilderTemplate::And(std::string_view where_clause)
    -> SelectQueryBuilderTemplate & {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" AND ({})", where_clause);
  return *this;
}
auto SelectQueryBuilderTemplate::Or(std::string_view where_clause)
    -> SelectQueryBuilderTemplate & {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" OR ({})", where_clause);
  return *this;
}

auto SelectQueryBuilderTemplate::Limit(int limit)
    -> SelectQueryBuilderTemplate & {
  Expects(std::holds_alternative<std::monostate>(limit_.value));
  limit_.value.emplace<int>(limit);
  Ensures(!std::holds_alternative<std::monostate>(limit_.value));
  return *this;
}

auto SelectQueryBuilderTemplate::LimitParam() -> SelectQueryBuilderTemplate & {
  Expects(std::holds_alternative<std::monostate>(limit_.value));
  limit_.value.emplace<LimitedType>();
  Ensures(!std::holds_alternative<std::monostate>(limit_.value));
  return *this;
}

auto SelectQueryBuilderTemplate::Build() const
    -> std::pair<Query, std::vector<CellDefinition>> {
  Expects(!table_name_.empty());
  Expects(!cell_definitions_.empty());

  auto query = std::string{"SELECT "};

  if (select_all_) {
    query += "*";
  } else if (select_one_) {
    query += "1";
  } else {
    Expects(!full_column_names_.empty());
    query += full_column_names_;
  }

  query += fmt::format(" FROM {}{}{}{}", table_name_, join_clause_,
                       where_clause_, limit_.GetLimitClause());

  return {{std::move(query)}, cell_definitions_};
}
}  // namespace stonks::sqldb::qbf