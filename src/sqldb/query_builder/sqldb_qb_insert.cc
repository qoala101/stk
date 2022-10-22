#include "sqldb_qb_insert.h"

#include <gsl/assert>
#include <memory>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qb_common.h"

namespace stonks::sqldb::qb {
auto Insert::Build() const -> Query {
  Expects(!table_name_.empty());

  auto query = fmt::format("INSERT INTO {} ({}) VALUES ({})", table_name_,
                           column_names_, values_);

  Ensures(!query.empty());
  return {std::move(query)};
}
}  // namespace stonks::sqldb::qb