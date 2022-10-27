#include "sqldb_qb_condition.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <string>
#include <utility>

#include "cpp_typed_struct.h"
#include "sqldb_p_types.h"

namespace stonks::sqldb::qb {
Condition::Condition(p::Parametrized<Query> query)
    : query_{[&query]() {
        Expects(!query.value.empty());
        return std::move(query);
      }()} {
  Ensures(!query_.value.empty());
}

auto Condition::operator&&(const Condition &condition) -> Condition & {
  AppendCondition(condition, "AND");
  return *this;
}

auto Condition::operator||(const Condition &condition) -> Condition & {
  AppendCondition(condition, "OR");
  return *this;
}

auto Condition::GetQueryImpl(cpp::This<Condition> auto &t) -> auto & {
  return t.query_;
}

auto Condition::GetQuery() const -> const p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}

auto Condition::GetQuery() -> p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}

void Condition::AppendCondition(const Condition &condition,
                                std::string_view operator_string) {
  query_.value +=
      fmt::format(" {} ({})", operator_string, condition.query_.value);
  query_.params.Append(condition.query_.params);
}
}  // namespace stonks::sqldb::qb