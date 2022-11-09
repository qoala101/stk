#ifndef STONKS_SQLDB_PARAMETRIZED_SQLDB_P_COMMON_H_
#define STONKS_SQLDB_PARAMETRIZED_SQLDB_P_COMMON_H_

#include <vector>

#include "sqldb_p_types.h"
#include "sqldb_value.h"

namespace stonks::sqldb::p {
/**
 * @brief Validates whether values match the param types.
 */
auto AreParamsValid [[nodiscard]] (const std::vector<Value> &values,
                                   const std::vector<QueryParam> &params)
-> bool;
}  // namespace stonks::sqldb::p

#endif  // STONKS_SQLDB_PARAMETRIZED_SQLDB_P_COMMON_H_
