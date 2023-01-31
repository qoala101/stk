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
