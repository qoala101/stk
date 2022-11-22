#ifndef STONKS_SQLDB_SQLDB_VALUE_CONVERSIONS_H_
#define STONKS_SQLDB_SQLDB_VALUE_CONVERSIONS_H_

#include <type_traits>
#include <utility>
#include <vector>

#include "cpp_move_if.h"
#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_value.h"

namespace stonks::sqldb {
namespace detail {
template <typename T>
auto AsValue [[nodiscard]] (T &&t) {
  if constexpr (std::is_constructible_v<Value, T>) {
    return Value{std::forward<T>(t)};
  } else {
    return ConvertToValue(std::forward<T>(t));
  }
}
}  // namespace detail

/**
 * @brief Transforms arguments to values.
 * @remark In order to make the type convertible to the value, client must
 * define ConvertToValue which takes the type argument.
 */
template <typename... Args>
auto AsValues(Args &&...args) {
  return std::vector<Value>{detail::AsValue(std::forward<Args>(args))...};
}

/**
 * @brief Parses user type from the value.
 */
template <typename T>
struct ValueParser {
  using Type = T;

  auto operator() [[nodiscard]] (const Value &value) const -> Type;
  auto operator() [[nodiscard]] (Value &&value) const -> Type;
};

/**
 * @brief Transforms value to specified type.
 * @remark In order to make the value convertible to the type, client must
 * provide definition of the operator of the value parser.
 */
template <typename T, cpp::Is<Value> ValueT>
auto ValueAs [[nodiscard]] (ValueT &&value) -> T {
  if constexpr (SupportedDataType<T>) {
    return cpp::MoveIfRvalue<decltype(std::forward<ValueT>(value))>(
        value.template Get<T>());
  } else {
    return ValueParser<T>{}(std::forward<ValueT>(value));
  }
}
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_VALUE_CONVERSIONS_H_
