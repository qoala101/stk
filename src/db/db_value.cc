#include "db_value.h"

#include <absl/base/macros.h>

#include <type_traits>
#include <utility>

#include "db_enums.h"

namespace stonks::db {
Value::Value(std::string_view value, DataType data_type) {
  switch (data_type) {
    case DataType::kInt:
      value_ = std::stoi(value.data());
    case DataType::kInt64:
      value_ = std::stoll(value.data());
    case DataType::kDouble:
      value_ = std::stod(value.data());
    case DataType::kString:
      value_ = value.data();
    default:
      break;
  }
}

Value::Value(int value) : value_{value} {}

Value::Value(int64_t value) : value_{value} {}

Value::Value(double value) : value_{value} {}

Value::Value(std::string_view value) : value_{value.data()} {}

auto Value::GetInt() const -> int { return std::get<int>(value_); }

auto Value::GetInt64() const -> int64_t { return std::get<int64_t>(value_); }

auto Value::GetDouble() const -> double { return std::get<double>(value_); }

auto Value::GetString() const& -> const std::string& {
  return std::get<std::string>(value_);
}

auto Value::GetString() && -> std::string&& {
  return std::move(std::get<std::string>(value_));
}

auto Value::GetType() const -> DataType {
  return std::visit(
      [](const auto& variant) {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, int>) {
          return DataType::kInt;
        }

        if constexpr (std::is_same_v<T, int64_t>) {
          return DataType::kInt64;
        }

        if constexpr (std::is_same_v<T, double>) {
          return DataType::kDouble;
        }

        if constexpr (std::is_same_v<T, std::string>) {
          return DataType::kString;
        }

        ABSL_ASSERT(false && "Unknown type");
      },
      value_);
}

auto Value::ToString() const -> std::string {
  return std::visit(
      [](const auto& variant) -> std::string {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, int64_t> ||
                      std::is_same_v<T, double>) {
          return std::to_string(variant);
        }

        if constexpr (std::is_same_v<T, std::string>) {
          return "\"" + variant + "\"";
        }

        return {};
      },
      value_);
}
}  // namespace stonks::db