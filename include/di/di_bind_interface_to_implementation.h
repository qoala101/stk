#ifndef STONKS_DI_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_
#define STONKS_DI_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_

#include <boost/di.hpp>
#include <boost/di/extension/injections/factory.hpp>

#include "cpp_not_null.h"
#include "di_make_injector.h"

namespace stonks::di {
namespace detail {
template <typename T>
struct AssumedNn : public cpp::Nn<T> {
  explicit AssumedNn(T t) : cpp::Nn<T>{cpp::AssumeNn(std::move(t))} {}
};

template <typename Interface>
auto EnableNn [[nodiscard]] () {
  return MakeInjector(
      boost::di::bind<cpp::NnUp<Interface>>()
          .template to<detail::AssumedNn<cpp::Up<Interface>>>(),
      boost::di::bind<cpp::NnSp<Interface>>()
          .template to<detail::AssumedNn<cpp::Sp<Interface>>>());
}

template <typename Interface, std::derived_from<Interface> Implementation>
auto EnableFactory [[nodiscard]] () {
  return MakeInjector(
      boost::di::bind<boost::di::extension::ifactory<Interface>>().to(
          boost::di::extension::factory<Implementation>{}),
      detail::EnableNn<boost::di::extension::ifactory<Interface>>());
}
}  // namespace detail

/**
 * @brief Binds interface to implementation and enables creation of not null
 * and interface factory.
 */
template <typename Interface, std::derived_from<Interface> Implementation>
auto BindInterfaceToImplementation [[nodiscard]] () {
  return MakeInjector(
      boost::di::bind<Interface>().template to<Implementation>().in(
          boost::di::unique),
      detail::EnableNn<Interface>(),
      detail::EnableFactory<Interface, Implementation>());
}
}  // namespace stonks::di

#endif  // STONKS_DI_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_
