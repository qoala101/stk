#ifndef STONKS_SQLDB_SQLDB_ENUMS_TO_STRING_H_
#define STONKS_SQLDB_SQLDB_ENUMS_TO_STRING_H_

#include <magic_enum.hpp>
#include <string_view>

#include "sqldb_enums.h"

template <>
[[nodiscard]] constexpr auto
magic_enum::customize::enum_name<stonks::sqldb::DataType>(
    stonks::sqldb::DataType value) noexcept -> std::string_view {
  switch (value) {
    case stonks::sqldb::DataType::kInt:
    case stonks::sqldb::DataType::kInt64:
      return "INTEGER";
    case stonks::sqldb::DataType::kDouble:
      return "REAL";
    case stonks::sqldb::DataType::kString:
      return "TEXT";
  }

  return {};
}

#endif  // STONKS_SQLDB_SQLDB_ENUMS_TO_STRING_H_
