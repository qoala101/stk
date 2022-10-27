#ifndef STONKS_SQLDB_PARAMETRIZED_SQLDB_P_COMMON_H_
#define STONKS_SQLDB_PARAMETRIZED_SQLDB_P_COMMON_H_

#include "sqldb_p_types.h"
#include "sqldb_value.h"

namespace stonks::sqldb::p {
/**
 * @brief Validates whether values match the param types.
 */
[[nodiscard]] auto AreParamsValid(const std::vector<Value> &values,
                                  const std::vector<Param> &params) -> bool;
}  // namespace stonks::sqldb::p

#endif  // STONKS_SQLDB_PARAMETRIZED_SQLDB_P_COMMON_H_