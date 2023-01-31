#include "sqldb_types.h"

#include <not_null.hpp>
#include <variant>

namespace vh::sqldb {
auto operator<=>(const DataTypeVariant& left, const DataTypeVariant& right)
    -> std::partial_ordering {
  return left->index() <=> right->index();
}
}  // namespace vh::sqldb