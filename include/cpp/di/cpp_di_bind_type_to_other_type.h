#ifndef STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_OTHER_TYPE_H_
#define STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_OTHER_TYPE_H_

#include <boost/di.hpp>
#include <type_traits>

namespace stonks::cpp::di {
namespace detail {
template <typename Type, std::convertible_to<Type> Injected>
struct InjectedFactory {
  [[nodiscard]] auto operator()(const auto &injector) const -> Type {
    return injector.template create<Injected>();
  }
};
}  // namespace detail

/**
 * @brief Binds type to other type.
 * @remark Can be used to bind variant to specific variant.
 */
template <typename Type, std::convertible_to<Type> Injected>
[[nodiscard]] auto BindTypeToOtherType() {
  return boost::di::bind<Type>().to(detail::InjectedFactory<Type, Injected>{});
}
}  // namespace stonks::cpp::di

#endif  // STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_OTHER_TYPE_H_
