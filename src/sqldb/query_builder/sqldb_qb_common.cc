#include "sqldb_qb_common.h"

#include <fmt/format.h>

#include "sqldb_qb_select.h"

namespace stonks::sqldb::qb {
auto On(const Condition &where) -> OnCondition { return OnCondition{where}; };

auto Exists(const Select &query_builder) -> Condition {
  auto query = query_builder.Build();
  return Condition{{fmt::format("EXISTS ({})", *query), query.params}};
};
}  // namespace stonks::sqldb::qb
