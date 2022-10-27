#include "sqldb_qb_delete.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <string>
#include <utility>

namespace stonks::sqldb::qb {
auto Delete::Where(WhereCondition condition) -> Delete& {
  auto& where_query = condition.GetQuery();
  Expects(!where_query.value.empty());
  where_query_ = std::move(where_query);
  Ensures(!where_query_.value.empty());
  return *this;
}

auto Delete::Build() const -> p::Parametrized<Query> {
  return {
      fmt::format("DELETE FROM {}{}", table_name_.value, where_query_.value),
      where_query_.params};
}

Delete::Delete(std::string table_name)
    : table_name_{[&table_name]() {
        Expects(!table_name.empty());
        return std::move(table_name);
      }()} {
  Ensures(!table_name_.value.empty());
}
}  // namespace stonks::sqldb::qb