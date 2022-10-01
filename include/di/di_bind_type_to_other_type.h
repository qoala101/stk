#ifndef STONKS_DI_DI_BIND_TYPE_TO_OTHER_TYPE_H_
#define STONKS_DI_DI_BIND_TYPE_TO_OTHER_TYPE_H_

#include <boost/di.hpp>
#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep

namespace stonks::di {
namespace detail {
template <typename Type, typename Injected>
  requires cpp::ConstructibleFrom<Type, Injected>
struct TypeInjector : public Type {
  explicit TypeInjector(Injected injected) : Type{std::move(injected)} {}
};
}  // namespace detail

/**
 * @brief Binds type to other type.
 * @remark Can be used to bind variant to specific variant.
 */
template <typename Type, typename Injected>
  requires cpp::ConstructibleFrom<Type, Injected>
[[nodiscard]] auto BindTypeToOtherType() {
  return boost::di::bind<Type>()
      .template to<detail::TypeInjector<Type, Injected>>();
}
}  // namespace stonks::di

#endif  // STONKS_DI_DI_BIND_TYPE_TO_OTHER_TYPE_H_
