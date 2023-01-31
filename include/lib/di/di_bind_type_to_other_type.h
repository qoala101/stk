#ifndef STONKS_DI_DI_BIND_TYPE_TO_OTHER_TYPE_H_
#define STONKS_DI_DI_BIND_TYPE_TO_OTHER_TYPE_H_

#include <boost/di.hpp>
#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep

namespace vh::di {
namespace detail {
template <typename T, typename Injected>
  requires cpp::ConstructibleFrom<T, Injected>
struct TypeInjector : public T {
  explicit TypeInjector(Injected injected) : T{std::move(injected)} {}
};
}  // namespace detail

/**
 * @brief Binds type to other type.
 * @remark Can be used to bind variant to specific variant.
 */
template <typename T, typename Injected>
  requires cpp::ConstructibleFrom<T, Injected>
auto BindTypeToOtherType [[nodiscard]] () {
  return boost::di::bind<T>().template to<detail::TypeInjector<T, Injected>>();
}
}  // namespace vh::di

#endif  // STONKS_DI_DI_BIND_TYPE_TO_OTHER_TYPE_H_
