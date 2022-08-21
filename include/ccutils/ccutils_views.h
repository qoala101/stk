#ifndef STONKS_CCUTILS_CCUTILS_VIEWS_H_
#define STONKS_CCUTILS_CCUTILS_VIEWS_H_

#include <vector>

#include "ccutils_not_null.h"

namespace stonks::ccutils {
/**
 * @brief Pointers to multiple elements.
 */
template <typename T>
using View = std::vector<ccutils::Nn<T *>>;

/**
 * @brief Const pointers to multiple elements.
 */
template <typename T>
using ConstView = View<const T>;
}  // namespace stonks::ccutils

#endif  // STONKS_CCUTILS_CCUTILS_VIEWS_H_
