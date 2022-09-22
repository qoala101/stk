#ifndef STONKS_SQLDB_SQLDB_AS_VALUES_H_
#define STONKS_SQLDB_SQLDB_AS_VALUES_H_

#include "sqldb_value.h"

namespace stonks::sqldb {
/**
 * @brief Transforms arguments to values.
 */
template <typename... Ts>
auto AsValues(Ts &&...ts) -> std::vector<Value> {
  return {Value{std::forward<Ts>(ts)}...};
}
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_AS_VALUES_H_
