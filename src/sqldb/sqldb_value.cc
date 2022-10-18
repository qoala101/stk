#include "sqldb_value.h"

#include <gsl/assert>
#include <variant>

#include "sqldb_enums.h"

namespace stonks::sqldb {
Value::Value(std::string_view value, DataType data_type) {
  // NOLINTNEXTLINE(*-simplify-boolean-expr)
  Expects(!value.empty() || (data_type == DataType::kString));

  switch (data_type) {
    case DataType::kBool:
      this->value = static_cast<bool>(std::stoi(value.data()));
    case DataType::kInt:
      this->value = std::stoi(value.data());
    case DataType::kInt64:
      this->value = std::stoll(value.data());
    case DataType::kDouble:
      this->value = std::stod(value.data());
    case DataType::kString:
      this->value = value.data();
  }

  Ensures(!IsNull());
}

auto Value::GetBool() const -> bool {
  Expects(std::holds_alternative<bool>(value));
  return std::get<bool>(value);
}

auto Value::GetInt() const -> int {
  Expects(std::holds_alternative<int>(value));
  return std::get<int>(value);
}

auto Value::GetInt64() const -> int64_t {
  Expects(std::holds_alternative<int64_t>(value));
  return std::get<int64_t>(value);
}

auto Value::GetDouble() const -> double {
  Expects(std::holds_alternative<double>(value));
  return std::get<double>(value);
}

auto Value::GetStringImpl(cpp::This<Value> auto& t) -> auto& {
  Expects(std::holds_alternative<std::string>(t.value));
  return std::get<std::string>(t.value);
}

auto Value::GetString() const -> const std::string& {
  return GetStringImpl(*this);
}

auto Value::GetString() -> std::string& { return GetStringImpl(*this); }

auto Value::GetType() const -> DataType {
  Expects(!IsNull());
  return std::visit(
      [](const auto& v) -> DataType {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, bool>) {
          return DataType::kBool;
        } else if constexpr (cpp::DecaysTo<V, int>) {
          return DataType::kInt;
        } else if constexpr (cpp::DecaysTo<V, int64_t>) {
          return DataType::kInt64;
        } else if constexpr (cpp::DecaysTo<V, double>) {
          return DataType::kDouble;
        } else if constexpr (cpp::DecaysTo<V, std::string>) {
          return DataType::kString;
        } else {
          return {};
        }
      },
      value);
}

auto Value::IsNull() const -> bool {
  return std::holds_alternative<std::monostate>(value);
}
}  // namespace stonks::sqldb