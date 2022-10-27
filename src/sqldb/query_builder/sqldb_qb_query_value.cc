#include "sqldb_qb_query_value.h"

namespace stonks::sqldb::qb {
QueryValue::QueryValue(const Value &value) : query_{value.ToString()} {}

QueryValue::QueryValue(p::Param param) : query_{"?", {{{param}}}} {}

QueryValue::QueryValue(const Select &select) : QueryValue{select.Build()} {}

QueryValue::QueryValue(p::Parametrized<SelectQuery> query)
    : query_{fmt::format("({})", query.value), std::move(query.params)} {}

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