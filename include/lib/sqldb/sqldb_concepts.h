#ifndef STONKS_SQLDB_SQLDB_CONCEPTS_H_
#define STONKS_SQLDB_SQLDB_CONCEPTS_H_

#include <concepts>
#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "sqldb_types.h"

namespace vh::sqldb {
/**
 * @brief Data types supported by library.
 */
template <typename T>
concept SupportedDataType =
    cpp::VariantOf<DataType<T>, DataTypeVariant::ValueType>;

/**
 * @brief Type which defines a SQL table.
 */
template <typename T>
concept TableDefinition = requires { cpp::IsTypeList<typename T::Columns>; };

/**
 * @brief Type which defines a SQL table column.
 */
template <typename T>
concept ColumnDefinition = requires {
                             TableDefinition<typename T::Table>;
                             SupportedDataType<typename T::DataType>;
                           };
}  // namespace vh::sqldb

#endif  // STONKS_SQLDB_SQLDB_CONCEPTS_H_
