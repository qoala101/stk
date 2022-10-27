#include "sqldb_qb_on_condition.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <string>

#include "sqldb_p_types.h"

namespace stonks::sqldb::qb {
OnCondition::OnCondition(const Condition &condition)
    : query_{[&condition]() {
        const auto &query = condition.GetQuery();
        Expects(!query.value.empty());
        return p::Parametrized<Query>{fmt::format(" ON ({})", query.value),
                                      query.params};
      }()} {
  Ensures(!query_.value.empty());
}

auto OnCondition::GetQueryImpl(cpp::This<OnCondition> auto &t) -> auto & {
  return t.query_;
}

auto OnCondition::GetQuery() const -> const p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}

auto OnCondition::GetQuery() -> p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}
}  // namespace stonks::sqldb::qb