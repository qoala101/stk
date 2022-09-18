#ifndef STONKS_CPP_DI_CPP_DI_MAKE_INJECTOR_H_
#define STONKS_CPP_DI_CPP_DI_MAKE_INJECTOR_H_

#include <boost/di.hpp>

namespace stonks::cpp::di {
/**
 * @brief Makes injector.
 */
template <typename... Args>
[[nodiscard]] auto MakeInjector(Args &&...args) {
  return boost::di::make_injector(std::forward<Args>(args)...);
}
}  // namespace stonks::cpp::di

#endif  // STONKS_CPP_DI_CPP_DI_MAKE_INJECTOR_H_
