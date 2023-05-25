/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_qb_drop.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <not_null.hpp>
#include <utility>

#include "cpp_typed_struct.h"

namespace vh::sqldb::qb {
auto Drop::Build() const -> Query {
  return {fmt::format("DROP TABLE {}", *table_name_)};
}

Drop::Drop(std::string table_name)
    : table_name_{(Expects(!table_name.empty()), std::move(table_name))} {
  Ensures(!table_name_->empty());
}
}  // namespace vh::sqldb::qb