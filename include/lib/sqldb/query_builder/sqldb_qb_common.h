/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLDB_QB_COMMON_H_
#define VH_SQLDB_QB_COMMON_H_

#include "cpp_meta_template_constructor.h"
#include "sqldb_concepts.h"
#include "sqldb_prm_types.h"
#include "sqldb_qb_condition.h"
#include "sqldb_qb_query_value.h"
#include "sqldb_qb_wrapped_conditions.h"
#include "sqldb_types.h"

namespace vh::sqldb::qb {
/**
 * @brief Adds ON to condition.
 */
auto On [[nodiscard]] (const Condition &where) -> OnCondition;

/**
 * @brief Adds EXISTS to select query.
 */
auto Exists [[nodiscard]] (const Select &query_builder) -> Condition;

/**
 * @brief Gives column name as value.
 */
template <ColumnDefinition ColumnT>
auto Column [[nodiscard]] () -> QueryValue {
  return QueryValue{cpp::meta::TemplateConstructor<ColumnT>{}};
}

/**
 * @brief Creates a query param of specified type.
 */
template <SupportedDataType T>
auto ParamOfType [[nodiscard]] () -> QueryValue {
  return prm::QueryParam{DataType<T>{}};
}

/**
 * @brief Creates a query param of column type.
 */
template <ColumnDefinition Column>
auto ParamForColumn [[nodiscard]] () -> QueryValue {
  return ParamOfType<typename Column::DataType>();
}
}  // namespace vh::sqldb::qb

#endif  // VH_SQLDB_QB_COMMON_H_
