#include "sqldb_qb_delete.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <string>
#include <utility>

namespace stonks::sqldb::qb {
auto Delete::Where(std::string_view where_clause)
    -> Delete& {
  Expects(where_clause_.empty());
  where_clause_ = fmt::format(" WHERE ({})", where_clause);
  Ensures(!where_clause_.empty());
  return *this;
}

auto Delete::And(std::string_view where_clause)
    -> Delete& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" AND ({})", where_clause);
  return *this;
}
auto Delete::Or(std::string_view where_clause)
    -> Delete& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" OR ({})", where_clause);
  return *this;
}

auto Delete::Build() const -> Query {
  Expects(!table_name_.empty());

  auto query = std::string{"DELETE "};

  query += fmt::format(" FROM {}{}", table_name_, where_clause_);

  return {query};
}
}  // namespace stonks::sqldb::qb