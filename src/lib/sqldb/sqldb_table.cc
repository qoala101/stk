/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_table.h"

#include <fmt/core.h>

#include <string>

namespace vh::sqldb::detail {
auto GetFullNameInTableImpl(std::string_view table_name,
                            std::string_view column_name) -> std::string {
  return fmt::format("{}.{}", table_name, column_name);
}
}  // namespace vh::sqldb::detail