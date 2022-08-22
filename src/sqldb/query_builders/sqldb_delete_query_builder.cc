#include "sqldb_delete_query_builder.h"

#include <gsl/assert>
#include <memory>
#include <string>
#include <utility>

namespace stonks::sqldb::query_builder_facade {
DeleteQueryBuilder::DeleteQueryBuilder(
    ccutils::NnSp<IQueryBuilder> query_builder)
    : query_builder_{std::move(query_builder)} {}

auto DeleteQueryBuilder::FromTable(Table table) -> DeleteQueryBuilder& {
  Expects(!table_.has_value());
  table_.emplace(std::move(table));
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
  where_clause_.emplace(std::string{"WHERE "} + where_clause.data());
  Ensures(where_clause_.has_value());
  return *this;
}

auto DeleteQueryBuilder::Build() const -> std::string {
  Expects(table_.has_value());
  return query_builder_->BuildDeleteQuery(
      *table_, where_clause_.value_or(std::string{}));
}
}  // namespace stonks::sqldb::query_builder_facade