/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLDB_CONCEPTS_H_
#define VH_SQLDB_CONCEPTS_H_

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

#endif  // VH_SQLDB_CONCEPTS_H_
