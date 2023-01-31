#ifndef STONKS_DI_DI_BIND_VALUE_TYPE_TO_VALUE_H_
#define STONKS_DI_DI_BIND_VALUE_TYPE_TO_VALUE_H_

#include <type_traits>
#include <utility>

#include "di_bind_type_to_value.h"

namespace vh::di {
/**
 * @brief Binds type of the value to value.
 */
template <typename Value>
auto BindValueTypeToValue [[nodiscard]] (Value &&value) {
  return BindTypeToValue<std::remove_cvref_t<Value>>(
      std::forward<Value>(value));
}
}  // namespace vh::di

#endif  // STONKS_DI_DI_BIND_VALUE_TYPE_TO_VALUE_H_
