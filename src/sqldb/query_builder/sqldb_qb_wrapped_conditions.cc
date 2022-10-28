#include "sqldb_qb_wrapped_conditions.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <string>

#include "sqldb_p_types.h"

namespace stonks::sqldb::qb {
WrappedCondition::WrappedCondition(const Condition &condition,
                                   std::string_view format)
    : query_{[&condition, format]() {
        const auto &query = condition.GetQuery();
        Expects(!query.value.empty());
        return p::Parametrized<Query>{
            fmt::format(fmt::runtime(format), query.value), query.params};
      }()} {
  Ensures(!query_.value.empty());
}

auto WrappedCondition::GetQueryImpl(cpp::This<WrappedCondition> auto &t)
    -> auto & {
  return t.query_;
}

auto WrappedCondition::GetQuery() const -> const p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}

auto WrappedCondition::GetQuery() -> p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}

OnCondition::OnCondition(const Condition &condition)
    : WrappedCondition{condition, " ON ({})"} {}

WhereCondition::WhereCondition(const Condition &condition)
    : WrappedCondition{condition, " WHERE ({})"} {}
}  // namespace stonks::sqldb::qb