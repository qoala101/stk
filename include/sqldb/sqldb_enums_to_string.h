#ifndef STONKS_SQLDB_SQLDB_ENUMS_TO_STRING_H_
#define STONKS_SQLDB_SQLDB_ENUMS_TO_STRING_H_

#include <magic_enum.hpp>
#include <string_view>

#include "sqldb_enums.h"

/**
 * @file Library enums to string conversions.
 */

template <>
[[nodiscard]] constexpr auto
magic_enum::customize::enum_name<stonks::sqldb::DataType>(
    stonks::sqldb::DataType value) noexcept
    -> magic_enum::customize::customize_t {
  switch (value) {
    case stonks::sqldb::DataType::kBool:
    case stonks::sqldb::DataType::kInt:
    case stonks::sqldb::DataType::kInt64:
      return "INTEGER";
    case stonks::sqldb::DataType::kDouble:
      return "REAL";
    case stonks::sqldb::DataType::kString:
      return "TEXT";
  }

  return invalid_tag;
}

#endif  // STONKS_SQLDB_SQLDB_ENUMS_TO_STRING_H_
