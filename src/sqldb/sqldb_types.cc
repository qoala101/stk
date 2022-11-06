#include "sqldb_types.h"

namespace stonks::sqldb {
[[nodiscard]] auto operator<=>(const DataTypeVariant& left,
                               const DataTypeVariant& right)
    -> std::partial_ordering {
  return left->index() <=> right->index();
}
}  // namespace stonks::sqldb