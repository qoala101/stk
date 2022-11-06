#include "sqldb_value.h"

#include <cstdint>
#include <gsl/assert>
#include <type_traits>
#include <variant>

#include "sqldb_types.h"

namespace stonks::sqldb {
template <>
auto Value::Get<bool>() const -> bool {
  Expects(std::holds_alternative<bool>(value));
  return std::get<bool>(value);
}

template <>
auto Value::Get<int>() const -> int {
  Expects(std::holds_alternative<int>(value));
  return std::get<int>(value);
}

template <>
auto Value::Get<int64_t>() const -> int64_t {
  Expects(std::holds_alternative<int64_t>(value));
  return std::get<int64_t>(value);
}

template <>
auto Value::Get<double>() const -> double {
  Expects(std::holds_alternative<double>(value));
  return std::get<double>(value);
}

template <SupportedDataType T>
auto Value::GetImpl(cpp::This<Value> auto& t) -> auto& {
  Expects(std::holds_alternative<T>(*t));
  return std::get<T>(*t);
}

template <>
auto Value::Get<std::string>() const -> const std::string& {
  return GetImpl<std::string>(*this);
}

template <>
auto Value::Get<std::string>() -> std::string& {
  return GetImpl<std::string>(*this);
}

auto Value::GetType() const -> DataTypeVariant {
  return std::visit(
      [](const auto& v) -> DataTypeVariant {
        using V = std::decay_t<decltype(v)>;

        if constexpr (!std::is_same_v<V, std::monostate>) {
          return {DataType<V>{}};
        } else {
          Expects(false);
        }
      },
      value);
}

auto Value::IsNull() const -> bool {
  return std::holds_alternative<std::monostate>(value);
}
}  // namespace stonks::sqldb