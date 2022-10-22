#include "sqldb_qb_create.h"

#include <bits/ranges_algo.h>

#include <gsl/assert>
#include <magic_enum.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/view/filter.hpp>

namespace stonks::sqldb::qb {
auto Create::IfNotExists() -> Create & {
  Expects(if_not_exists_clause_.empty());
  if_not_exists_clause_ = " IF NOT EXISTS";
  Ensures(!if_not_exists_clause_.empty());
  return *this;
}

auto Create::Build() const -> Query {
  auto query = fmt::format(R"(CREATE TABLE{} "{}"({}{}{}))",
                           if_not_exists_clause_, table_name_, columns_clause_,
                           primary_key_query_, foreign_keys_query_);
  return {std::move(query)};
}
}  // namespace stonks::sqldb::qb