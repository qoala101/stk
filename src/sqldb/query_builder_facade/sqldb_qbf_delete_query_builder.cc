#include "sqldb_qbf_delete_query_builder.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <string>
#include <utility>

namespace stonks::sqldb::qbf {
DeleteQueryBuilder::DeleteQueryBuilder(cpp::NnUp<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto DeleteQueryBuilder::FromTable(Table table) -> DeleteQueryBuilder& {
  Expects(!table_.has_value());
  table_ = std::move(table);
  Ensures(table_.has_value());
  return *this;
}

auto DeleteQueryBuilder::FromTable(const TableDefinition& table_definition)
    -> DeleteQueryBuilder& {
  return FromTable(table_definition.table);
}

auto DeleteQueryBuilder::Where(std::string_view where_clause)
    -> DeleteQueryBuilder& {
  Expects(!where_clause_.has_value());
  where_clause_ = fmt::format("WHERE {}", where_clause);
  Ensures(where_clause_.has_value());
  return *this;
}

auto DeleteQueryBuilder::Build() const -> Query {
  Expects(table_.has_value());
  return query_builder_->BuildDeleteQuery(
      *table_, where_clause_.value_or(std::string{}));
}

auto DeleteQueryBuilderTemplate::Where(std::string_view where_clause)
    -> DeleteQueryBuilderTemplate& {
  Expects(where_clause_.empty());
  where_clause_ = fmt::format(" WHERE ({})", where_clause);
  Ensures(!where_clause_.empty());
  return *this;
}

auto DeleteQueryBuilderTemplate::And(std::string_view where_clause)
    -> DeleteQueryBuilderTemplate& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" AND ({})", where_clause);
  return *this;
}
auto DeleteQueryBuilderTemplate::Or(std::string_view where_clause)
    -> DeleteQueryBuilderTemplate& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" OR ({})", where_clause);
  return *this;
}

auto DeleteQueryBuilderTemplate::Build() const -> Query {
  Expects(!table_name_.empty());

  auto query = std::string{"DELETE "};

  query += fmt::format(" FROM {}{}", table_name_, where_clause_);

  return {query};
}
}  // namespace stonks::sqldb::qbf