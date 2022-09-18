#ifndef STONKS_CPP_DI_CPP_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_
#define STONKS_CPP_DI_CPP_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_

#include <boost/di.hpp>

#include "cpp_di_factory.h"
#include "cpp_di_make_injector.h"
#include "cpp_not_null.h"

namespace stonks::cpp::di {
namespace detail {
template <typename T>
struct AssumedNn : public Nn<T> {
  explicit AssumedNn(T t) : Nn<T>{AssumeNn(std::move(t))} {}
};

template <typename Interface>
[[nodiscard]] auto EnableNn() {
  return MakeInjector(::boost::di::bind<NnUp<Interface>>()
                          .template to<detail::AssumedNn<Up<Interface>>>(),
                      ::boost::di::bind<NnSp<Interface>>()
                          .template to<detail::AssumedNn<Sp<Interface>>>());
}

template <typename Interface, std::derived_from<Interface> Implementation>
[[nodiscard]] auto EnableFactory() {
  return MakeInjector(
      boost::di::bind<IFactory<Interface>>().to(Factory<Implementation>{}),
      detail::EnableNn<IFactory<Interface>>());
}
}  // namespace detail

/**
 * @brief Binds interface to implementation and enables creation of not null
 * and interface factory.
 */
template <typename Interface, std::derived_from<Interface> Implementation>
[[nodiscard]] auto BindInterfaceToImplementation() {
  return MakeInjector(
      boost::di::bind<Interface>().template to<Implementation>(),
      detail::EnableNn<Interface>(),
      detail::EnableFactory<Interface, Implementation>());
}
}  // namespace stonks::cpp::di

#endif  // STONKS_CPP_DI_CPP_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_
