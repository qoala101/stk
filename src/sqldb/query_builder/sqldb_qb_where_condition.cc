#include "sqldb_qb_where_condition.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <string>

#include "sqldb_p_types.h"

namespace stonks::sqldb::qb {
WhereCondition::WhereCondition(const Condition &condition)
    : query_{[&condition]() {
        const auto &query = condition.GetQuery();
        Expects(!query.value.empty());
        return p::Parametrized<Query>{fmt::format(" WHERE ({})", query.value),
                                      query.params};
      }()} {
  Ensures(!query_.value.empty());
}

auto WhereCondition::GetQueryImpl(cpp::This<WhereCondition> auto &t) -> auto & {
  return t.query_;
}

auto WhereCondition::GetQuery() const -> const p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}

auto WhereCondition::GetQuery() -> p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}
}  // namespace stonks::sqldb::qb