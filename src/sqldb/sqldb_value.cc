#include "sqldb_value.h"

#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

#include "sqldb_enums.h"

namespace stonks::sqldb {
Value::Value(const char* value) : Value{std::string{value}} {}

Value::Value(std::string_view value, DataType data_type) {
  // NOLINTNEXTLINE(*-simplify-boolean-expr)
  Expects(!value.empty() || (data_type == DataType::kString));

  switch (data_type) {
    case DataType::kBool:
      *this = static_cast<bool>(std::stoi(value.data()));
    case DataType::kInt:
      *this = std::stoi(value.data());
    case DataType::kInt64:
      *this = std::stoll(value.data());
    case DataType::kDouble:
      *this = std::stod(value.data());
    case DataType::kString:
      *this = value.data();
  }

  Ensures(!IsNull());
}

auto Value::GetBool() const -> bool {
  Expects(std::holds_alternative<bool>(*this));
  return std::get<bool>(*this);
}

auto Value::GetInt() const -> int {
  Expects(std::holds_alternative<int>(*this));
  return std::get<int>(*this);
}

auto Value::GetInt64() const -> int64_t {
  Expects(std::holds_alternative<int64_t>(*this));
  return std::get<int64_t>(*this);
}

auto Value::GetDouble() const -> double {
  Expects(std::holds_alternative<double>(*this));
  return std::get<double>(*this);
}

template <cpp::DecaysTo<Value> This>
auto Value::GetStringImpl(This& t) -> cpp::CopyConst<This, std::string> & {
  Expects(std::holds_alternative<std::string>(t));
  return std::get<std::string>(t);
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
        }

        if constexpr (cpp::DecaysTo<V, int>) {
          return DataType::kInt;
        }

        if constexpr (cpp::DecaysTo<V, int64_t>) {
          return DataType::kInt64;
        }

        if constexpr (cpp::DecaysTo<V, double>) {
          return DataType::kDouble;
        }

        if constexpr (cpp::DecaysTo<V, std::string>) {
          return DataType::kString;
        }

        return {};
      },
      static_cast<const detail::ValueVariantType&>(*this));
}

auto Value::IsNull() const -> bool {
  return std::holds_alternative<std::monostate>(*this);
}
}  // namespace stonks::sqldb