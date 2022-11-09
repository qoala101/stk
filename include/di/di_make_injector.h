#ifndef STONKS_DI_DI_MAKE_INJECTOR_H_
#define STONKS_DI_DI_MAKE_INJECTOR_H_

#include <boost/di.hpp>

namespace stonks::di {
/**
 * @brief Makes injector.
 * @tparam Args Boost DI bindings or other injectors.
 */
template <typename... Ts, typename... Args>
auto MakeInjector [[nodiscard]] (Args &&...args) {
  return boost::di::make_injector<Ts...>(std::forward<Args>(args)...);
}
}  // namespace stonks::di

#endif  // STONKS_DI_DI_MAKE_INJECTOR_H_
