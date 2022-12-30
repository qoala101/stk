#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_

#include "cpp_template_constructor.h"
#include "sqldb_concepts.h"
#include "sqldb_p_types.h"
#include "sqldb_qb_condition.h"
#include "sqldb_qb_query_value.h"
#include "sqldb_qb_wrapped_conditions.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
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
  return QueryValue{cpp::TemplateConstructor<ColumnT>{}};
}

/**
 * @brief Creates a query param of specified type.
 */
template <SupportedDataType T>
auto ParamOfType [[nodiscard]] () -> QueryValue {
  return p::QueryParam{DataType<T>{}};
}

/**
 * @brief Creates a query param of column type.
 */
template <ColumnDefinition Column>
auto ParamForColumn [[nodiscard]] () -> QueryValue {
  return ParamOfType<typename Column::DataType>();
}
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_
