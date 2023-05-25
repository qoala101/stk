/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_qb_common.h"

#include <fmt/core.h>

#include <string>

#include "cpp_typed_struct.h"
#include "sqldb_qb_select.h"

namespace vh::sqldb::qb {
auto On(const Condition &where) -> OnCondition { return OnCondition{where}; };

auto Exists(const Select &query_builder) -> Condition {
  auto query = query_builder.Build();
  return Condition{{fmt::format("EXISTS ({})", *query), query.params}};
};
}  // namespace vh::sqldb::qb
