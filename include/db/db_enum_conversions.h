#ifndef STONKS_DB_DB_ENUM_CONVERSIONS_H_
#define STONKS_DB_DB_ENUM_CONVERSIONS_H_

#include <magic_enum.hpp>
#include <string_view>

#include "db_enums.h"

template <>
constexpr std::string_view
magic_enum::customize::enum_name<stonks::db::DataType>(
    stonks::db::DataType value) noexcept {
  switch (value) {
    case stonks::db::DataType::kInteger:
      return "INTEGER";
    case stonks::db::DataType::kText:
      return "TEXT";
    case stonks::db::DataType::kReal:
      return "REAL";
    default:
      break;
  }

  return {};
}

#endif  // STONKS_DB_DB_ENUM_CONVERSIONS_H_
