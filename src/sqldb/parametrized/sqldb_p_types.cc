#include "sqldb_p_types.h"

namespace stonks::sqldb::p {
void QueryParams::operator+=(const QueryParams& other) {
  value.insert(value.end(), other.value.begin(), other.value.end());
}
}  // namespace stonks::sqldb::p