#ifndef STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_OTHER_TYPE_H_
#define STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_OTHER_TYPE_H_

#include <boost/di.hpp>
#include <type_traits>

namespace stonks::di {
namespace detail {
template <typename Type, std::convertible_to<Type> Injected>
struct TypeInjector : public Type {
  explicit TypeInjector(Injected injected) : Type{std::move(injected)} {}
};
}  // namespace detail

/**
 * @brief Binds type to other type.
 * @remark Can be used to bind variant to specific variant.
 */
template <typename Type, std::convertible_to<Type> Injected>
[[nodiscard]] auto BindTypeToOtherType() {
  return boost::di::bind<Type>()
      .template to<detail::TypeInjector<Type, Injected>>();
}
}  // namespace stonks::di

#endif  // STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_OTHER_TYPE_H_
