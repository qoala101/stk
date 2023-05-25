#include "sqldb_prm_types.h"

#include <not_null.hpp>

namespace vh::sqldb::prm {
void QueryParams::operator+=(const QueryParams& other) {
  (*this)->insert((*this)->end(), other->cbegin(), other->cend());
}
}  // namespace vh::sqldb::prm