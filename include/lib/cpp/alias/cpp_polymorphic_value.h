/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_POLYMORPHIC_VALUE_H_
#define VH_CPP_POLYMORPHIC_VALUE_H_

#include <polymorphic_value.h>

#include <utility>

/**
 * @file Short isocpp_p0201::polymorphic_value aliases.
 */

namespace vh::cpp {
template <typename... Ts>
using Pv = isocpp_p0201::polymorphic_value<Ts...>;

template <typename... Ts, typename... Args>
auto MakePv [[nodiscard]] (Args &&...args) {
  return isocpp_p0201::make_polymorphic_value<Ts...>(
      std::forward<Args>(args)...);
}
}  // namespace vh::cpp

#endif  // VH_CPP_POLYMORPHIC_VALUE_H_
