#include "sqldb_qb_wrapped_conditions.h"

#include <fmt/core.h>

#include "cpp_typed_struct.h"
#include "sqldb_prm_types.h"
#include "sqldb_qb_query_wrapper.h"
#include "sqldb_types.h"

namespace vh::sqldb::qb {
WrappedCondition::WrappedCondition(const Condition &condition,
                                   std::string_view format)
    : QueryWrapper{[&condition, format]() {
        const auto &query = condition.GetQuery();
        return prm::Parametrized<Query>{fmt::format(fmt::runtime(format), *query),
                                      query.params};
      }()} {}

OnCondition::OnCondition(const Condition &condition)
    : WrappedCondition{condition, " ON ({})"} {}

WhereCondition::WhereCondition(const Condition &condition)
    : WrappedCondition{condition, " WHERE ({})"} {}
}  // namespace vh::sqldb::qb