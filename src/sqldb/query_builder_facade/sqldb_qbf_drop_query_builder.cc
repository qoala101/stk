#include "sqldb_qbf_drop_query_builder.h"

#include <bits/ranges_algo.h>

#include <gsl/assert>
#include <magic_enum.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/view/filter.hpp>

namespace stonks::sqldb::qbf {
auto DropQueryBuilderTemplate::Build() const -> Query {
  Expects(!table_name_.empty());
  auto query = fmt::format("DROP TABLE {}", table_name_);
  return {std::move(query)};
}
}  // namespace stonks::sqldb::qbf