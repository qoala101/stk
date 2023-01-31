#ifndef VH_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_
#define VH_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_

#include <boost/di.hpp>
#include <boost/di/extension/injections/factory.hpp>
#include <type_traits>

#include "cpp_factory.h"
#include "cpp_meta_thread_safe.h"
#include "cpp_mutex.h"
#include "cpp_not_null.h"
#include "di_bind_type_to_factory_function.h"
#include "di_enable_nn_pointers.h"
#include "di_make_injector.h"

namespace vh::di {
namespace detail {
template <typename Interface>
auto FactoryFrom
    [[nodiscard]] (cpp::NnSp<boost::di::extension::ifactory<Interface>> factory,
                   cpp::MutexVariant mutex) {
  return cpp::Factory<Interface>{[factory = std::move(factory)]() {
                                   return cpp::AssumeNn(factory->create());
                                 },
                                 std::move(mutex)};
}

template <cpp::Interface Interface, std::derived_from<Interface> Implementation>
auto EnableFactory [[nodiscard]] () {
  return MakeInjector(
      boost::di::bind<boost::di::extension::ifactory<Interface>>().to(
          boost::di::extension::factory<Implementation>{}),
      detail::EnableNnPointers<boost::di::extension::ifactory<Interface>>(),
      di::BindTypeToFactoryFunction<
          cpp::Factory<Interface>,
          +[](cpp::NnSp<boost::di::extension::ifactory<Interface>> factory) {
            return FactoryFrom(std::move(factory), {});
          }>(),
      di::BindTypeToFactoryFunction<
          cpp::meta::ThreadSafe<cpp::Factory<Interface>>,
          +[](cpp::NnSp<boost::di::extension::ifactory<Interface>> factory) {
            return cpp::meta::AssumeThreadSafe(
                FactoryFrom(std::move(factory), {cpp::Mutex{}}));
          }>());
}
}  // namespace detail

/**
 * @brief Binds interface to implementation and enables creation of not null
 * and interface factory.
 */
template <cpp::Interface Interface, std::derived_from<Interface> Implementation>
auto BindInterfaceToImplementation [[nodiscard]] () {
  return MakeInjector(
      boost::di::bind<Interface>().template to<Implementation>().in(
          boost::di::unique),
      detail::EnableNnPointers<Interface>(),
      detail::EnableFactory<Interface, Implementation>());
}
}  // namespace vh::di

#endif  // VH_DI_BIND_INTERFACE_TO_IMPLEMENTATION_H_
