/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_prm_types.h"

#include <not_null.hpp>

namespace vh::sqldb::prm {
void QueryParams::operator+=(const QueryParams& other) {
  (*this)->insert((*this)->end(), other->cbegin(), other->cend());
}
}  // namespace vh::sqldb::prm