#include "sqldb_table.h"

#include <fmt/core.h>

#include <string>

namespace stonks::sqldb::detail {
auto GetFullNameInTableImpl(std::string_view table_name,
                            std::string_view column_name) -> std::string {
  return fmt::format("{}.{}", table_name, column_name);
}
}  // namespace stonks::sqldb::detail