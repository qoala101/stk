#ifndef STONKS_DI_DI_ENABLE_NN_POINTERS_H_
#define STONKS_DI_DI_ENABLE_NN_POINTERS_H_

#include <boost/di.hpp>
#include <type_traits>

#include "cpp_not_null.h"
#include "di_make_injector.h"

namespace stonks::di {
namespace detail {
template <typename T>
struct AssumedNn : public cpp::Nn<T> {
  explicit AssumedNn(T t) : cpp::Nn<T>{cpp::AssumeNn(std::move(t))} {}
};

template <typename T>
auto EnableNnPointers [[nodiscard]] () {
  return MakeInjector(boost::di::bind<cpp::NnUp<T>>()
                          .template to<detail::AssumedNn<cpp::Up<T>>>(),
                      boost::di::bind<cpp::NnSp<T>>()
                          .template to<detail::AssumedNn<cpp::Sp<T>>>());
}
}  // namespace detail

/**
 * @brief Enables creation of not null pointers of non-polymorphic type.
 * @remark For polymorphic types not null pointers are automatically enabled
 * when binding implementation to interface.
 */
template <typename T>
  requires(!std::is_polymorphic_v<T>)
auto EnableNnPointers [[nodiscard]] () {
  return detail::EnableNnPointers<T>();
}
}  // namespace stonks::di

#endif  // STONKS_DI_DI_ENABLE_NN_POINTERS_H_
