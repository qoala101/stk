#include "sqldb_qb_delete.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <string>
#include <utility>

namespace stonks::sqldb::qb {
auto Delete::Where(const WhereQuery& where) -> Delete& {
  Expects(where_query_.empty());
  where_query_ = fmt::format(" WHERE ({})", where.value);
  Ensures(!where_query_.empty());
  return *this;
}

auto Delete::And(const WhereQuery& where) -> Delete& {
  Expects(!where_query_.empty());
  where_query_ += fmt::format(" AND ({})", where.value);
  return *this;
}
auto Delete::Or(const WhereQuery& where) -> Delete& {
  Expects(!where_query_.empty());
  where_query_ += fmt::format(" OR ({})", where.value);
  return *this;
}

auto Delete::Build() const -> Query {
  auto query = fmt::format("DELETE FROM {}{}", table_name_, where_query_);
  Ensures(!query.empty());
  return {std::move(query)};
}

Delete::Delete(std::string table_name)
    : table_name_{[&table_name]() {
        Expects(!table_name.empty());
        return std::move(table_name);
      }()} {
  Ensures(!table_name_.empty());
}
}  // namespace stonks::sqldb::qb