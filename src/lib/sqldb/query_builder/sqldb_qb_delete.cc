/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_qb_delete.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <not_null.hpp>
#include <string>
#include <utility>

#include "cpp_typed_struct.h"

namespace vh::sqldb::qb {
auto Delete::Where(WhereCondition condition) -> Delete& {
  Expects(where_query_->empty());

  auto& condition_query = condition.GetQuery();
  Expects(!condition_query->empty());

  where_query_ = std::move(condition_query);
  Ensures(!where_query_->empty());
  return *this;
}

auto Delete::Build() const -> prm::Parametrized<Query> {
  return {fmt::format("DELETE FROM {}{}", *table_name_, *where_query_),
          where_query_.params};
}

Delete::Delete(std::string table_name)
    : table_name_{(Expects(!table_name.empty()), std::move(table_name))} {
  Ensures(!table_name_->empty());
}
}  // namespace vh::sqldb::qb