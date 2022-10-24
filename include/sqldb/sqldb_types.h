#ifndef STONKS_SQLDB_SQLDB_TYPES_H_
#define STONKS_SQLDB_SQLDB_TYPES_H_

#include <string>
#include <vector>

#include "cpp_typed_struct.h"
#include "cpp_variant_struct.h"

namespace stonks::sqldb {
/**
 * @brief SQL query.
 */
struct Query : public cpp::TypedStruct<std::string> {};

/**
 * @brief Type of the data recognized by the library.
 */
template <typename T>
struct DataType {
  using Type = T;
};

/**
 * @brief Holds supported data types.
 */
struct DataTypeVariant
    : public cpp::VariantStruct<DataType<bool>, DataType<int>,
                                DataType<int64_t>, DataType<double>,
                                DataType<std::string>> {};
/**
 * @brief Table column name.
 */
struct Column : public cpp::TypedStruct<std::string> {};

/**
 * @brief SQL column type.
 */
struct ColumnType {
  Column column{};
  DataTypeVariant type{};
};

/**
 * @brief Definition of select statement result.
 */
struct ResultDefinition : public cpp::TypedStruct<std::vector<ColumnType>> {};

/**
 * @brief SQL query with expected result types.
 */
struct SelectQuery {
  Query query{};
  ResultDefinition result_definition{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TYPES_H_
