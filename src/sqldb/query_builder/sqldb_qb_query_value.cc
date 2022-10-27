#include "sqldb_qb_query_value.h"

#include <gsl/assert>

namespace stonks::sqldb::qb {
QueryValue::QueryValue(const Value &value) : query_{value.ToString()} {}

QueryValue::QueryValue(p::Param param) : query_{"?", {{{param}}}} {}

QueryValue::QueryValue(const Select &select) : QueryValue{select.Build()} {}

QueryValue::QueryValue(const p::Parametrized<SelectQuery> &query)
    : query_{[&query]() {
               Expects(!query.value.empty());
               return p::Parametrized<SelectQuery>{
                   fmt::format("({})", query.value)};
             }(),
             query.params} {
  Ensures(!query_.value.empty());
}

auto QueryValue::GetQueryImpl(cpp::This<QueryValue> auto &t) -> auto & {
  return t.query_;
}

[[nodiscard]] auto QueryValue::GetQuery() const
    -> const p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}

[[nodiscard]] auto QueryValue::GetQuery() -> p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}
}  // namespace stonks::sqldb::qb