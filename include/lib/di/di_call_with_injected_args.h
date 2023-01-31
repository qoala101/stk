#ifndef VH_DI_CALL_WITH_INJECTED_ARGS_H_
#define VH_DI_CALL_WITH_INJECTED_ARGS_H_

#include <type_traits>

namespace vh::di {
/**
 * @brief Calls the function with arguments which are created by injector.
 */
template <typename... Args>
auto CallWithInjectedArgs
    [[nodiscard]] (auto(*function)(Args...), const auto &injector) {
  return function(injector.template create<std::remove_cvref_t<Args>>()...);
}
}  // namespace vh::di

#endif  // VH_DI_CALL_WITH_INJECTED_ARGS_H_
