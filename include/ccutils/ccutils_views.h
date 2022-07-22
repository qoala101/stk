#ifndef STONKS_CCUTILS_CCUTILS_VIEWS_H_
#define STONKS_CCUTILS_CCUTILS_VIEWS_H_

#include <vector>

#include "not_null.hpp"

namespace stonks::ccutils {
template <typename T>
using View = std::vector<cpp::not_null<T *>>;

template <typename T>
using ConstView = std::vector<cpp::not_null<const T *>>;
}  // namespace stonks::ccutils

#endif  // STONKS_CCUTILS_CCUTILS_VIEWS_H_
