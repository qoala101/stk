#include "sqldb_value.h"

#include <absl/base/macros.h>

#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

#include "sqldb_enums.h"

namespace stonks::sqldb {
Value::Value(std::string_view value, DataType data_type) {
  Expects(!value.empty() || (data_type == DataType::kString));

  switch (data_type) {
    case DataType::kInt:
      value_ = std::stoi(value.data());
    case DataType::kInt64:
      value_ = std::stoll(value.data());
    case DataType::kDouble:
      value_ = std::stod(value.data());
    case DataType::kString:
      value_ = value.data();
  }
}

Value::Value(int value) : value_{value} {}

Value::Value(int64_t value) : value_{value} {}

Value::Value(double value) : value_{value} {}

Value::Value(std::string_view value) : value_{value.data()} {}

auto Value::GetInt() const -> int {
  Expects(std::holds_alternative<int>(value_));
  return std::get<int>(value_);
}

auto Value::GetInt64() const -> int64_t {
  Expects(std::holds_alternative<int64_t>(value_));
  return std::get<int64_t>(value_);
}

auto Value::GetDouble() const -> double {
  Expects(std::holds_alternative<double>(value_));
  return std::get<double>(value_);
}

auto Value::GetString() const -> const std::string& {
  Expects(std::holds_alternative<std::string>(value_));
  return std::get<std::string>(value_);
}

auto Value::GetString() -> std::string& {
  // NOLINTNEXTLINE(*-const-cast)
  return const_cast<std::string&>(const_cast<const Value*>(this)->GetString());
}

auto Value::GetType() const -> DataType {
  Expects(!IsNull());

  return std::visit(
      [](const auto& variant) -> DataType {
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

        return {};
      },
      value_);
}

[[nodiscard]] auto Value::IsNull() const -> bool {
  return std::holds_alternative<std::monostate>(value_);
}
}  // namespace stonks::sqldb