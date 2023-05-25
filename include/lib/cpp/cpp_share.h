/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_SHARE_H_
#define VH_CPP_SHARE_H_

#include <type_traits>

#include "cpp_not_null.h"

namespace vh::cpp {
/**
 * @brief Explicit conversion to shared pointer.
 */
template <typename T>
auto Share(T t) {
  return MakeNnSp<std::remove_cvref_t<T>>(std::move(t));
}

/**
 * @copydoc Share
 */
template <typename T>
auto Share(Up<T> t) {
  return Sp<T>{std::move(t)};
}

/**
 * @copydoc Share
 */
template <typename T>
auto Share(NnUp<T> t) {
  return NnSp<T>{std::move(t)};
}
}  // namespace vh::cpp

#endif  // VH_CPP_SHARE_H_
