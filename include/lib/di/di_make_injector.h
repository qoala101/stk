/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_DI_MAKE_INJECTOR_H_
#define VH_DI_MAKE_INJECTOR_H_

#include <boost/di.hpp>

namespace vh::di {
/**
 * @brief Makes injector.
 * @tparam Args Boost DI bindings or other injectors.
 */
template <typename... Ts, typename... Args>
auto MakeInjector [[nodiscard]] (Args &&...args) {
  return boost::di::make_injector<Ts...>(std::forward<Args>(args)...);
}
}  // namespace vh::di

#endif  // VH_DI_MAKE_INJECTOR_H_
