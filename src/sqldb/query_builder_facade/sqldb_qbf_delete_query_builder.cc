#include "sqldb_qbf_delete_query_builder.h"

#include <gsl/assert>
#include <memory>
#include <string>
#include <utility>

#include "cpp_format.h"

namespace stonks::sqldb::qbf {
DeleteQueryBuilder::DeleteQueryBuilder(cpp::NnSp<IQueryBuilder> query_builder)
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
  where_clause_ = cpp::Format("WHERE {}", where_clause);
  Ensures(where_clause_.has_value());
  return *this;
}

auto DeleteQueryBuilder::Build() const -> Query {
  Expects(table_.has_value());
  return query_builder_->BuildDeleteQuery(
      *table_, where_clause_.value_or(std::string{}));
}
}  // namespace stonks::sqldb::qbf