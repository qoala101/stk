#include "sqldb_qb_drop.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <utility>

#include "cpp_typed_struct.h"
#include "not_null.hpp"

namespace stonks::sqldb::qb {
auto Drop::Build() const -> Query {
  return {fmt::format("DROP TABLE {}", *table_name_)};
}

Drop::Drop(std::string table_name)
    : table_name_{[&table_name]() {
        Expects(!table_name.empty());
        return std::move(table_name);
      }()} {
  Ensures(!table_name_->empty());
}
}  // namespace stonks::sqldb::qb