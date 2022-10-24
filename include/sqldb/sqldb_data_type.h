#ifndef STONKS_SQLDB_SQLDB_DATA_TYPE_H_
#define STONKS_SQLDB_SQLDB_DATA_TYPE_H_

/**
 * @file Library enums.
 */

#include <cstdint>
#include <gsl/assert>
#include <string>

#include "cpp_variant_struct.h"

namespace stonks::sqldb {
/**
 * @brief
 */
template <typename T>
struct DataType {};

/**
 * @brief
 */
struct DataTypeVariant
    : public cpp::VariantStruct<DataType<bool>, DataType<int>,
                                DataType<int64_t>, DataType<double>,
                                DataType<std::string>> {};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_DATA_TYPE_H_
