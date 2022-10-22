#include "sqldb_qb_update.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qb_common.h"

namespace stonks::sqldb::qb {
auto Update::Where(std::string_view where_clause)
    -> Update& {
  Expects(where_clause_.empty());
  where_clause_ = fmt::format(" WHERE ({})", where_clause);
  Ensures(!where_clause_.empty());
  return *this;
}

auto Update::And(std::string_view where_clause)
    -> Update& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" AND ({})", where_clause);
  return *this;
}
auto Update::Or(std::string_view where_clause)
    -> Update& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" OR ({})", where_clause);
  return *this;
}

auto Update::Build() const -> Query {
  Expects(!table_name_.empty());
  Expects(!column_values_.empty());
  auto query = fmt::format("UPDATE {} SET {}{}", table_name_, column_values_,
                           where_clause_);
  return {std::move(query)};
}
}  // namespace stonks::sqldb::qb