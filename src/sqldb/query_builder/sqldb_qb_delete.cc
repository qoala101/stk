#include "sqldb_qb_delete.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <string>
#include <utility>

namespace stonks::sqldb::qb {
auto Delete::Where(const WhereQuery& where) -> Delete& {
  Expects(where_clause_.empty());
  where_clause_ = fmt::format(" WHERE ({})", where.value);
  Ensures(!where_clause_.empty());
  return *this;
}

auto Delete::And(const WhereQuery& where) -> Delete& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" AND ({})", where.value);
  return *this;
}
auto Delete::Or(const WhereQuery& where) -> Delete& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" OR ({})", where.value);
  return *this;
}

auto Delete::Build() const -> Query {
  Expects(!table_name_.empty());

  auto query = std::string{"DELETE "};

  query += fmt::format(" FROM {}{}", table_name_, where_clause_);

  return {query};
}

Delete::Delete(std::string table_name) : table_name_{std::move(table_name)} {
  Ensures(!table_name_.empty());
}
}  // namespace stonks::sqldb::qb