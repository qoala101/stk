/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_VIEWS_H_
#define VH_CPP_VIEWS_H_

#include <vector>

#include "cpp_not_null.h"

namespace vh::cpp {
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
}  // namespace vh::cpp

#endif  // VH_CPP_VIEWS_H_
