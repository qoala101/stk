#ifndef STONKS_CCUTILS_CCUTILS_MOVE_IF_RVALUE_H_
#define STONKS_CCUTILS_CCUTILS_MOVE_IF_RVALUE_H_

#include <concepts>
#include <gsl/util>
#include <iostream>
#include <type_traits>
#include <utility>

namespace stonks::ccutils {
/**
 * @brief Applies rvalue cast to the value.
 */
template <typename T>
requires std::is_rvalue_reference_v<T>
auto MoveIfThisIsRvalue(auto &value) -> auto && { return std::move(value); }

/**
 * @brief Returns the value as it is.
 */
template <typename T>
auto MoveIfThisIsRvalue(auto &&value) -> auto && {
  return std::forward<decltype(value)>(value);
}

/**
 * @brief Applies rvalue cast to value if first argument is rvalue.
 */
auto MoveIfThisIsRvalue(const auto &&t, auto &&value) -> auto && {
  return MoveIfThisIsRvalue<decltype(t)>(std::forward<decltype(value)>(value));
}
}  // namespace stonks::ccutils

#endif  // STONKS_CCUTILS_CCUTILS_MOVE_IF_RVALUE_H_
