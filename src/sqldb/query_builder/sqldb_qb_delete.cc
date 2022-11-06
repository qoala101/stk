#include "sqldb_qb_delete.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <string>
#include <utility>

namespace stonks::sqldb::qb {
auto Delete::Where(WhereCondition condition) -> Delete& {
  Expects(where_query_->empty());

  auto& condition_query = condition.GetQuery();
  Expects(!condition_query->empty());

  where_query_ = std::move(condition_query);
  Ensures(!where_query_->empty());
  return *this;
}

auto Delete::Build() const -> p::Parametrized<Query> {
  return {fmt::format("DELETE FROM {}{}", *table_name_, *where_query_),
          where_query_.params};
}

Delete::Delete(std::string table_name)
    : table_name_{[&table_name]() {
        Expects(!table_name.empty());
        return std::move(table_name);
      }()} {
  Ensures(!table_name_->empty());
}
}  // namespace stonks::sqldb::qb