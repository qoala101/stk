#ifndef STONKS_CCUTILS_CCUTILS_VIEWS_H_
#define STONKS_CCUTILS_CCUTILS_VIEWS_H_

#include <vector>

#include "not_null.hpp"

namespace stonks::ccutils {
/**
 * @brief Pointers to multiple elements.
 */
template <typename T>
using View = std::vector<cpp::not_null<T *>>;

/**
 * @brief Const pointers to multiple elements.
 */
template <typename T>
using ConstView = View<const T>;
}  // namespace stonks::ccutils

#endif  // STONKS_CCUTILS_CCUTILS_VIEWS_H_
