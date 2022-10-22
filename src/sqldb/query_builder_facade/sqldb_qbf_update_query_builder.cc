#include "sqldb_qbf_update_query_builder.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qbf_common.h"

namespace stonks::sqldb::qbf {
auto UpdateQueryBuilderTemplate::Where(std::string_view where_clause)
    -> UpdateQueryBuilderTemplate& {
  Expects(where_clause_.empty());
  where_clause_ = fmt::format(" WHERE ({})", where_clause);
  Ensures(!where_clause_.empty());
  return *this;
}

auto UpdateQueryBuilderTemplate::And(std::string_view where_clause)
    -> UpdateQueryBuilderTemplate& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" AND ({})", where_clause);
  return *this;
}
auto UpdateQueryBuilderTemplate::Or(std::string_view where_clause)
    -> UpdateQueryBuilderTemplate& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" OR ({})", where_clause);
  return *this;
}

auto UpdateQueryBuilderTemplate::Build() const -> Query {
  Expects(!table_name_.empty());
  Expects(!column_values_.empty());
  auto query = fmt::format("UPDATE {} SET {}{}", table_name_, column_values_,
                           where_clause_);
  return {std::move(query)};
}
}  // namespace stonks::sqldb::qbf