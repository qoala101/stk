#ifndef STONKS_CPP_CPP_MOVE_IF_H_
#define STONKS_CPP_CPP_MOVE_IF_H_

#include "cpp_concepts.h"  // IWYU pragma: keep

/**
 * @file Utils for conditional applying of rvalue cast.
 */

namespace stonks::cpp {
/**
 * @brief Returns the value as it is.
 */
template <typename T>
auto MoveIfRvalue(auto &&value) -> auto && {
  return std::forward<decltype(value)>(value);
}

/**
 * @brief Applies rvalue cast to the value.
 */
template <Rvalue T>
auto MoveIfRvalue(auto &value) -> auto && {
  return std::move(value);
}

/**
 * @brief Applies rvalue cast to the value.
 */
template <typename T>
auto MoveIfNotConst(auto &value) -> auto && {
  return std::move(value);
}

/**
 * @brief Returns the value as it is.
 */
template <Const T>
auto MoveIfNotConst(auto &value) -> auto && {
  return std::forward<decltype(value)>(value);
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_MOVE_IF_H_
