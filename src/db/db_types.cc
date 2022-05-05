#include "db_types.h"

#include "range/v3/algorithm/find_if.hpp"

namespace stonks::db {
std::optional<int> Value::GetInt() const {
  // TODO: replace int cast with real value
  return std::visit(
      [](const auto& variant) -> std::optional<int64_t> {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, int64_t>) {
          return static_cast<int>(variant);
        }

        return std::nullopt;
      },
      value);
}

std::optional<int64_t> Value::GetInt64() const {
  return std::visit(
      [](const auto& variant) -> std::optional<int64_t> {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, int64_t>) {
          return variant;
        }

        return std::nullopt;
      },
      value);
}

std::optional<double> Value::GetDouble() const {
  return std::visit(
      [](const auto& variant) -> std::optional<double> {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, double>) {
          return variant;
        }

        return std::nullopt;
      },
      value);
}

const std::string* Value::GetString() const {
  return std::visit(
      [](const auto& variant) -> const std::string* {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, std::string>) {
          return &variant;
        }

        return nullptr;
      },
      value);
}

std::string Value::ToString() const {
  return std::visit(
      [](const auto& variant) {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, int64_t>) {
          return std::to_string(variant);
        }

        if constexpr (std::is_same_v<T, double>) {
          return std::to_string(variant);
        }

        if constexpr (std::is_same_v<T, std::string>) {
          return "\"" + variant + "\"";
        }
      },
      value);
}

Value Value::FromString(std::string_view value, DataType data_type) {
  switch (data_type) {
    case DataType::kInteger:
      return Value{.value = std::stoll(value.data())};
    case DataType::kReal:
      return Value{.value = std::stod(value.data())};
    case DataType::kText:
      return Value{.value = std::string{value}};
    default:
      break;
  }

  return {};
}

const Value* Row::GetCellValue(std::string_view column_name) const {
  const auto cell = ranges::find_if(cells, [column_name](const Cell& cell) {
    return cell.column_name == column_name;
  });

  if (cell == cells.end()) {
    return nullptr;
  }

  return &cell->value;
}

std::optional<int> Row::GetCellValueInt(std::string_view column_name) const {
  const auto value = GetCellValue(column_name);

  if (value == nullptr) {
    return std::nullopt;
  }

  return value->GetInt();
}

std::optional<int64_t> Row::GetCellValueInt64(
    std::string_view column_name) const {
  const auto value = GetCellValue(column_name);

  if (value == nullptr) {
    return std::nullopt;
  }

  return value->GetInt64();
}

std::optional<double> Row::GetCellValueDouble(
    std::string_view column_name) const {
  const auto value = GetCellValue(column_name);

  if (value == nullptr) {
    return std::nullopt;
  }

  return value->GetDouble();
}

const std::string* Row::GetCellValueString(std::string_view column_name) const {
  const auto value = GetCellValue(column_name);

  if (value == nullptr) {
    return nullptr;
  }

  return value->GetString();
}
}  // namespace stonks::db