#ifndef STONKS_DB_DB_ENUM_CONVERSIONS_H_
#define STONKS_DB_DB_ENUM_CONVERSIONS_H_

#include <magic_enum.hpp>
#include <string_view>

#include "sqldb_enums.h"

template <>
constexpr auto magic_enum::customize::enum_name<stonks::sqldb::DataType>(
    stonks::sqldb::DataType value) noexcept -> std::string_view {
  switch (value) {
    case stonks::sqldb::DataType::kInt:
    case stonks::sqldb::DataType::kInt64:
      return "INTEGER";
    case stonks::sqldb::DataType::kDouble:
      return "REAL";
    case stonks::sqldb::DataType::kString:
      return "TEXT";
    default:
      break;
  }

  return {};
}

#endif  // STONKS_DB_DB_ENUM_CONVERSIONS_H_
