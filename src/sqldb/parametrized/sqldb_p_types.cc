#include "sqldb_p_types.h"

#include <not_null.hpp>

namespace stonks::sqldb::p {
void QueryParams::operator+=(const QueryParams& other) {
  value.insert(value.end(), other->begin(), other->end());
}
}  // namespace stonks::sqldb::p