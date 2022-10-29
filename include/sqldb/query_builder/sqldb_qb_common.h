#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_

#include "sqldb_qb_condition.h"
#include "sqldb_qb_query_value.h"
#include "sqldb_qb_wrapped_conditions.h"
#include "sqldb_table_traits.h"

namespace stonks::sqldb::qb {
class Select;

/**
 * @brief Adds ON to condition.
 */
[[nodiscard]] auto On(const Condition &where) -> OnCondition;

/**
 * @brief Adds EXISTS to select query.
 */
[[nodiscard]] auto Exists(const Select &select) -> Condition;

/**
 * @brief Gives column name as value.
 */
template <typename ColumnT>
[[nodiscard]] auto Column() -> QueryValue {
  return QueryValue{static_cast<ColumnT *>(nullptr)};
}
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_
