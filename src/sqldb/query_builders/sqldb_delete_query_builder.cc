#include "sqldb_delete_query_builder.h"

#include <gsl/assert>
#include <string>
#include <utility>

namespace stonks::sqldb {
DeleteQueryBuilder::DeleteQueryBuilder(
    cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto DeleteQueryBuilder::FromTable(Table table) -> DeleteQueryBuilder& {
  Expects(!table_.has_value());
  table_ = {std::move(table)};
  return *this;
}

auto DeleteQueryBuilder::FromTable(const TableDefinition& table_definition)
    -> DeleteQueryBuilder& {
  return FromTable(table_definition.table);
}

auto DeleteQueryBuilder::Where(std::string_view where_clause)
    -> DeleteQueryBuilder& {
  Expects(!where_clause_.has_value());
  where_clause_.emplace(std::string{"WHERE "} + where_clause.data());
  return *this;
}

auto DeleteQueryBuilder::Build() const -> std::string {
  Expects(table_.has_value());
  return query_builder_->BuildDeleteQuery(
      *table_, where_clause_.value_or(std::string{}));
}
}  // namespace stonks::sqldb