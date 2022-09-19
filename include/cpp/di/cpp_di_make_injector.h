#ifndef STONKS_CPP_DI_CPP_DI_MAKE_INJECTOR_H_
#define STONKS_CPP_DI_CPP_DI_MAKE_INJECTOR_H_

#include <boost/di.hpp>

namespace stonks::cpp::di {
/**
 * @brief Overrides binding by appending [Override] to it.
 */
constexpr boost::di::core::override Override{};

/**
 * @brief Makes injector.
 * @tparam Args Boost DI bindings or other injectors.
 */
template <typename... Ts, typename... Args>
[[nodiscard]] auto MakeInjector(Args &&...args) {
  return boost::di::make_injector<Ts...>(std::forward<Args>(args)...);
}
}  // namespace stonks::cpp::di

#endif  // STONKS_CPP_DI_CPP_DI_MAKE_INJECTOR_H_
