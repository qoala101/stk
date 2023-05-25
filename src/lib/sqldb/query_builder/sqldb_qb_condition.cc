/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_qb_condition.h"

#include <fmt/core.h>

#include <string>
#include <utility>

#include "cpp_typed_struct.h"
#include "sqldb_prm_types.h"
#include "sqldb_qb_query_wrapper.h"

namespace vh::sqldb::qb {
Condition::Condition(prm::Parametrized<Query> query)
    : QueryWrapper{std::move(query)} {}

auto Condition::operator&&(const Condition &other) -> Condition & {
  AppendCondition(other, "AND");
  return *this;
}

auto Condition::operator||(const Condition &other) -> Condition & {
  AppendCondition(other, "OR");
  return *this;
}

void Condition::AppendCondition(const Condition &condition,
                                std::string_view operator_string) {
  auto &this_query = GetQuery();
  const auto &condition_query = condition.GetQuery();

  *this_query += fmt::format(" {} ({})", operator_string, *condition_query);
  this_query.params += condition_query.params;
}
}  // namespace vh::sqldb::qb