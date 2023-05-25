/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_qb_types.h"

#include <fmt/core.h>

namespace vh::sqldb::qb::detail {
auto GetFullAliasName(std::string_view target_name, std::string_view alias_name)
    -> std::string {
  return fmt::format("{} AS {}", target_name, alias_name);
}
}  // namespace vh::sqldb::qb::detail