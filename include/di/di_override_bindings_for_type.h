#ifndef STONKS_DI_DI_OVERRIDE_BINDINGS_FOR_TYPE_H_
#define STONKS_DI_DI_OVERRIDE_BINDINGS_FOR_TYPE_H_

#include <boost/di/extension/injections/extensible_injector.hpp>

#include "di_make_injector.h"

namespace stonks::di {
/**
 * @brief Creates injector proxy with overriden bindings for type.
 * @tparam T When creating this type, specified bindings would be used
 * instead of the ones in the input injector.
 * @param injector Existing injector. Lifetime must be greater then lifetime
 * of resulting injector.
 * @param args Bindings to be used when creating type T.
 */
template <typename T, typename... Args>
auto OverrideBindingsForType(auto& injector, Args&&... args) {
  return MakeInjector(boost::di::extension::make_extensible(injector),
                      boost::di::injector<T>{MakeInjector(
                          boost::di::extension::make_extensible(injector),
                          std::forward<Args>(args)[Override]...)});
}
}  // namespace stonks::di

#endif  // STONKS_DI_DI_OVERRIDE_BINDINGS_FOR_TYPE_H_
