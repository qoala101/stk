#ifndef STONKS_CPP_CPP_VIEWS_H_
#define STONKS_CPP_CPP_VIEWS_H_

#include <vector>

#include "cpp_not_null.h"

namespace stonks::cpp {
/**
 * @brief Pointers to multiple elements.
 */
template <typename T>
using View = std::vector<Nn<T *>>;

/**
 * @brief Const pointers to multiple elements.
 */
template <typename T>
using ConstView = View<const T>;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_VIEWS_H_
