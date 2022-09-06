#ifndef STONKS_CPP_CPP_MOVE_IF_H_
#define STONKS_CPP_CPP_MOVE_IF_H_

#include "cpp_concepts.h"  // IWYU pragma: keep

/**
 * @file Utils for conditional applying of rvalue cast.
 */

namespace stonks::cpp {
/**
 * @brief Applies rvalue cast to the value if T is rvalue.
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
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_MOVE_IF_H_
