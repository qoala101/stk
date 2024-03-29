/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_types.h"

#include <not_null.hpp>
#include <variant>

namespace vh::sqldb {
auto operator<=>(const DataTypeVariant& left, const DataTypeVariant& right)
    -> std::partial_ordering {
  return left->index() <=> right->index();
}
}  // namespace vh::sqldb