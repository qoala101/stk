#ifndef STONKS_DI_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_
#define STONKS_DI_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_

#include <boost/di.hpp>
#include <boost/di/extension/injections/factory.hpp>

#include "di_enable_nn_pointers.h"
#include "di_make_injector.h"

namespace stonks::di {
namespace detail {
template <typename Interface, std::derived_from<Interface> Implementation>
auto EnableFactory [[nodiscard]] () {
  return MakeInjector(
      boost::di::bind<boost::di::extension::ifactory<Interface>>().to(
          boost::di::extension::factory<Implementation>{}),
      detail::EnableNnPointers<boost::di::extension::ifactory<Interface>>());
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
      detail::EnableNnPointers<Interface>(),
      detail::EnableFactory<Interface, Implementation>());
}
}  // namespace stonks::di

#endif  // STONKS_DI_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_
