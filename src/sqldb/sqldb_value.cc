#include "sqldb_value.h"

#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

#include "sqldb_enums.h"

namespace stonks::sqldb {
Value::Value(std::string_view value, DataType data_type) {
  Expects(!value.empty() || (data_type == DataType::kString));

  switch (data_type) {
    case DataType::kBool:
      value_ = static_cast<bool>(std::stoi(value.data()));
    case DataType::kInt:
      value_ = std::stoi(value.data());
    case DataType::kInt64:
      value_ = std::stoll(value.data());
    case DataType::kDouble:
      value_ = std::stod(value.data());
    case DataType::kString:
      value_ = value.data();
  }

  Ensures(!IsNull());
}

Value::Value(bool value) : value_{value} {}

Value::Value(int value) : value_{value} {}

Value::Value(int64_t value) : value_{value} {}

Value::Value(double value) : value_{value} {}

Value::Value(std::string value) : value_{std::move(value)} {}

auto Value::GetBool() const -> bool {
  Expects(std::holds_alternative<bool>(value_));
  return std::get<bool>(value_);
}

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

auto Value::GetStringImpl(auto& t) -> auto& {
  Expects(std::holds_alternative<std::string>(t.value_));
  return std::get<std::string>(t.value_);
}

auto Value::GetString() const -> const std::string& {
  return GetStringImpl(*this);
}

auto Value::GetString() -> std::string& { return GetStringImpl(*this); }

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

auto Value::IsNull() const -> bool {
  return std::holds_alternative<std::monostate>(value_);
}
}  // namespace stonks::sqldb