/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLDB_PRM_COMMON_H_
#define VH_SQLDB_PRM_COMMON_H_

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

#endif  // VH_SQLDB_PRM_COMMON_H_
