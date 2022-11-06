#include "sqldb_qb_query_wrapper.h"

#include <gsl/assert>

namespace stonks::sqldb::qb {
QueryWrapper::QueryWrapper(p::Parametrized<Query> query)
    : query_{[&query]() {
        Expects(!query->empty());
        return std::move(query);
      }()} {
  Ensures(!query_->empty());
}

auto QueryWrapper::GetQueryImpl(cpp::This<QueryWrapper> auto &t) -> auto & {
  return t.query_;
}

auto QueryWrapper::GetQuery() const -> const p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}

auto QueryWrapper::GetQuery() -> p::Parametrized<Query> & {
  return GetQueryImpl(*this);
}
}  // namespace stonks::sqldb::qb