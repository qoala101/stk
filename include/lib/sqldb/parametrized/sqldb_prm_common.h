#ifndef STONKS_SQLDB_PARAMETRIZED_SQLDB_P_COMMON_H_
#define STONKS_SQLDB_PARAMETRIZED_SQLDB_P_COMMON_H_

#include <vector>

#include "sqldb_prm_types.h"
#include "sqldb_value.h"

namespace vh::sqldb::prm {
/**
 * @brief Validates whether values match the param types.
 */
auto AreParamsValid [[nodiscard]] (const std::vector<Value> &values,
                                   const std::vector<QueryParam> &params)
-> bool;
}  // namespace vh::sqldb::prm

#endif  // STONKS_SQLDB_PARAMETRIZED_SQLDB_P_COMMON_H_
