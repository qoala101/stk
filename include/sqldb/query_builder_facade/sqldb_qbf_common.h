#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_COMMON_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_COMMON_H_

#include <variant>
#include <vector>

#include "cpp_views.h"
#include "sqldb_qbf_columns_variant.h"
#include "sqldb_qbf_table_variant.h"
#include "sqldb_types.h"

/**
 * @file Symbols used by query builders.
 */

namespace stonks::sqldb::qbf {
/**
 * @brief Gives columns from variants.
 */
[[nodiscard]] auto GetColumns(const TableVariant &table,
                              const ColumnsVariant &columns)
    -> std::vector<Column>;

/**
 * @brief Gives columns from their definitions.
 */
[[nodiscard]] auto GetColumns(
    const cpp::ConstView<ColumnDefinition> &column_definitions)
    -> std::vector<Column>;
}  // namespace stonks::sqldb::qbf

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_COMMON_H_
