#include "sqldb_types.h"

#include <memory>

#include "not_null.hpp"

namespace stonks::sqldb {
auto operator<=>
    [[nodiscard]] (const DataTypeVariant& left, const DataTypeVariant& right)
    -> std::partial_ordering {
  return left->index() <=> right->index();
}
}  // namespace stonks::sqldb