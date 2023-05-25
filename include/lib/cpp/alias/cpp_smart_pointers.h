/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_SMART_POINTERS_H_
#define VH_CPP_SMART_POINTERS_H_

#include <memory>

/**
 * @file Short smart pointer aliases.
 */

namespace vh::cpp {
template <typename... Ts>
using Up = std::unique_ptr<Ts...>;

template <typename... Ts>
using Sp = std::shared_ptr<Ts...>;

template <typename... Ts>
using Wp = std::weak_ptr<Ts...>;

template <typename... Ts, typename... Args>
auto MakeUp [[nodiscard]] (Args &&...args) {
  return std::make_unique<Ts...>(std::forward<Args>(args)...);
}

template <typename... Ts, typename... Args>
auto MakeSp [[nodiscard]] (Args &&...args) {
  return std::make_shared<Ts...>(std::forward<Args>(args)...);
}
}  // namespace vh::cpp

#endif  // VH_CPP_SMART_POINTERS_H_
