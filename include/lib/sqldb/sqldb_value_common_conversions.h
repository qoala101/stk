/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLDB_VALUE_COMMON_CONVERSIONS_H_
#define VH_SQLDB_VALUE_COMMON_CONVERSIONS_H_

#include "sqldb_value_conversions.h"

/**
 * @file Common conversions based on the base ones and standard templates.
 */

namespace vh::sqldb {
/**
 * @brief Parses descendants of typed structs.
 */
template <cpp::IsTypedStruct T>
struct ValueParser<T> {
  using Type = T;

 private:
  template <cpp::Is<Value> ValueT>
  static auto OperatorImpl [[nodiscard]] (ValueT &&value) {
    return Type{ValueAs<typename Type::ValueType>(std::forward<ValueT>(value))};
  }

 public:
  auto operator() [[nodiscard]] (const Value &value) const -> Type {
    return OperatorImpl(value);
  }

  auto operator() [[nodiscard]] (Value &&value) const -> Type {
    return OperatorImpl(std::move(value));
  }
};

/**
 * @brief Converts descendants of typed structs.
 */
template <typename T>
auto ConvertToValue [[nodiscard]] (const cpp::TypedStruct<T> &value) {
  return ConvertToValue(*value);
}
}  // namespace vh::sqldb

#endif  // VH_SQLDB_VALUE_COMMON_CONVERSIONS_H_
