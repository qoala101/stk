#include "sqldb_p_types.h"

namespace stonks::sqldb::p {
void Params::Append(const Params& params) {
  value.insert(value.end(), params.value.begin(), params.value.end());
}
}  // namespace stonks::sqldb::p