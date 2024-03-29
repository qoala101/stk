/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_MOVE_IF_H_
#define VH_CPP_MOVE_IF_H_

#include "cpp_concepts.h"  // IWYU pragma: keep

/**
 * @file Utils for conditional applying of rvalue cast.
 */

namespace vh::cpp {
/**
 * @brief Applies rvalue cast to the value if T is rvalue.
 * @remark Proper usage with forwarding reference:
 * @code {.cpp}
 * MoveIfRvalue<decltype(std::forward<This>(t))>(t.member);
 * @endcode
 */
template <typename T, typename U>
auto MoveIfRvalue(U &&value) -> auto && {
  if constexpr (Rvalue<T>) {
    // NOLINTNEXTLINE(*-move-forwarding-reference)
    return std::move(value);
  } else {
    return std::forward<U>(value);
  }
}

/**
 * @brief Applies rvalue cast to the value if T is not const.
 */
template <typename T, typename U>
auto MoveIfNotConst(U &&value) -> auto && {
  if constexpr (!Const<T>) {
    // NOLINTNEXTLINE(*-move-forwarding-reference)
    return std::move(value);
  } else {
    return std::forward<U>(value);
  }
}
}  // namespace vh::cpp

#endif  // VH_CPP_MOVE_IF_H_
