#include "sqldb_qb_condition.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <string>
#include <utility>

#include "cpp_typed_struct.h"
#include "sqldb_p_types.h"
#include "sqldb_qb_query_wrapper.h"

namespace stonks::sqldb::qb {
Condition::Condition(p::Parametrized<Query> query)
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

  this_query.value +=
      fmt::format(" {} ({})", operator_string, condition_query.value);
  this_query.params += condition_query.params;
}
}  // namespace stonks::sqldb::qb