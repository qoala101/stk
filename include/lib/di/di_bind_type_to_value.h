/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_DI_BIND_TYPE_TO_VALUE_H_
#define VH_DI_BIND_TYPE_TO_VALUE_H_

#include <boost/di.hpp>

namespace vh::di {
/**
 * @brief Binds type to value.
 */
template <typename T, typename Value>
auto BindTypeToValue [[nodiscard]] (Value &&value) {
  return boost::di::bind<T>().to(std::forward<Value>(value));
}
}  // namespace vh::di

#endif  // VH_DI_BIND_TYPE_TO_VALUE_H_
