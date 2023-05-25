/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_SWAP_RESET_H_
#define VH_CPP_SWAP_RESET_H_

#include <concepts>
#include <type_traits>
#include <utility>

namespace vh::cpp {
/**
 * @brief Util to reset the object state by swapping it with the empty one.
 * @param args Arguments to the new object constructor.
 */
template <typename T, typename... Args>
  requires std::constructible_from<T, Args...>
void SwapReset(T &t, Args &&...args) {
  using std::swap;

  auto other = T{std::forward<Args>(args)...};
  swap(t, other);
}
}  // namespace vh::cpp

#endif  // VH_CPP_SWAP_RESET_H_
