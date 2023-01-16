#include "sqldb_qb_drop.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <not_null.hpp>
#include <utility>

#include "cpp_typed_struct.h"

namespace stonks::sqldb::qb {
auto Drop::Build() const -> Query {
  return {fmt::format("DROP TABLE {}", *table_name_)};
}

Drop::Drop(std::string table_name)
    : table_name_{(Expects(!table_name.empty()), std::move(table_name))} {
  Ensures(!table_name_->empty());
}
}  // namespace stonks::sqldb::qb