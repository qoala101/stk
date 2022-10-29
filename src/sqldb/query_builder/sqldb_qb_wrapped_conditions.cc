#include "sqldb_qb_wrapped_conditions.h"

#include <fmt/core.h>

#include "sqldb_p_types.h"
#include "sqldb_qb_query_wrapper.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
WrappedCondition::WrappedCondition(const Condition &condition,
                                   std::string_view format)
    : QueryWrapper{[&condition, format]() {
        const auto &query = condition.GetQuery();
        return p::Parametrized<Query>{
            fmt::format(fmt::runtime(format), query.value), query.params};
      }()} {}

OnCondition::OnCondition(const Condition &condition)
    : WrappedCondition{condition, " ON ({})"} {}

WhereCondition::WhereCondition(const Condition &condition)
    : WrappedCondition{condition, " WHERE ({})"} {}
}  // namespace stonks::sqldb::qb