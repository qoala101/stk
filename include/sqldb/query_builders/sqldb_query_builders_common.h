#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QUERY_BUILDERS_COMMON_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QUERY_BUILDERS_COMMON_H_

#include <variant>
#include <vector>

#include "ccutils_views.h"
#include "sqldb_types.h"

/**
 * @file Symbols used by query builders.
 */
namespace stonks::sqldb::query_builder_facade {
/**
 * @brief Variant of columns specification.
 */
struct AllColumnsType {};

/**
 * @brief Stores table value in query builders.
 */
using TableVariant = std::variant<std::monostate, Table, TableDefinition>;

/**
 * @brief Stores columns value in query builders.
 */
using ColumnsVariant =
    std::variant<std::monostate, std::vector<Column>, AllColumnsType>;

/**
 * @brief Get table from variant.
 */
[[nodiscard]] auto GetTable(const TableVariant &table) -> Table;

/**
 * @brief Get columns from stored values.
 */
[[nodiscard]] auto GetColumns(const TableVariant &table,
                              const ColumnsVariant &columns)
    -> std::vector<Column>;

/**
 * @brief Get columns from their definitions.
 */
[[nodiscard]] auto GetColumns(
    const ccutils::ConstView<ColumnDefinition> &column_definitions)
    -> std::vector<Column>;
}  // namespace stonks::sqldb::query_builder_facade

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QUERY_BUILDERS_COMMON_H_
