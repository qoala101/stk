#include "sqldb_qb_drop.h"

#include <bits/ranges_algo.h>

#include <gsl/assert>
#include <magic_enum.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/view/filter.hpp>

namespace stonks::sqldb::qb {
auto Drop::Build() const -> Query {
  Expects(!table_name_.empty());
  auto query = fmt::format("DROP TABLE {}", table_name_);
  return {std::move(query)};
}
}  // namespace stonks::sqldb::qb