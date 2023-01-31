#include "sqldb_qb_query_wrapper.h"

#include <gsl/assert>
#include <not_null.hpp>
#include <string>
#include <utility>

#include "cpp_typed_struct.h"

namespace vh::sqldb::qb {
QueryWrapper::QueryWrapper(prm::Parametrized<Query> query)
    : query_{(Expects(!query->empty()), std::move(query))} {
  Ensures(!query_->empty());
}

auto QueryWrapper::GetQueryImpl(cpp::This<QueryWrapper> auto &t) -> auto & {
  return t.query_;
}

auto QueryWrapper::GetQuery() const -> const prm::Parametrized<Query> & {
  return GetQueryImpl(*this);
}

auto QueryWrapper::GetQuery() -> prm::Parametrized<Query> & {
  return GetQueryImpl(*this);
}
}  // namespace vh::sqldb::qb