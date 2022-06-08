#ifndef STONKS_DB_DB_ENUM_CONVERSIONS_H_
#define STONKS_DB_DB_ENUM_CONVERSIONS_H_

#include <magic_enum.hpp>
#include <string_view>

#include "db_enums.h"

template <>
constexpr auto magic_enum::customize::enum_name<stonks::db::DataType>(
    stonks::db::DataType value) noexcept -> std::string_view {
  switch (value) {
    case stonks::db::DataType::kInt:
    case stonks::db::DataType::kInt64:
      return "INTEGER";
    case stonks::db::DataType::kDouble:
      return "REAL";
    case stonks::db::DataType::kString:
      return "TEXT";
    default:
      break;
  }

  return {};
}

#endif  // STONKS_DB_DB_ENUM_CONVERSIONS_H_
