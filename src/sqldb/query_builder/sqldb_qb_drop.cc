#include "sqldb_qb_drop.h"

#include <gsl/assert>

namespace stonks::sqldb::qb {
auto Drop::Build() const -> Query {
  return {fmt::format("DROP TABLE {}", table_name_.value)};
}

Drop::Drop(std::string table_name)
    : table_name_{[&table_name]() {
        Expects(!table_name.empty());
        return std::move(table_name);
      }()} {
  Ensures(!table_name_.value.empty());
}
}  // namespace stonks::sqldb::qb