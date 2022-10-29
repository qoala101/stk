#include "sqldb_qb_query_value.h"

#include <gsl/assert>

#include "sqldb_qb_query_wrapper.h"
#include "sqldb_qb_select.h"

namespace stonks::sqldb::qb {
QueryValue::QueryValue(const Value &value) : QueryWrapper{{value.ToString()}} {}

QueryValue::QueryValue(p::Param param) : QueryWrapper{{{"?"}, {{{param}}}}} {}

QueryValue::QueryValue(const Select &select) : QueryValue{select.Build()} {}

QueryValue::QueryValue(const p::Parametrized<SelectQuery> &query)
    : QueryWrapper{{query.value, query.params}} {}
}  // namespace stonks::sqldb::qb