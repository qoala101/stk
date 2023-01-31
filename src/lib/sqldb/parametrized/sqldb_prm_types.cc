#include "sqldb_prm_types.h"

#include <not_null.hpp>

namespace vh::sqldb::prm {
void QueryParams::operator+=(const QueryParams& other) {
  (*this)->insert((*this)->end(), other->begin(), other->end());
}
}  // namespace vh::sqldb::prm