/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_DI_ENABLE_NN_POINTERS_H_
#define VH_DI_ENABLE_NN_POINTERS_H_

#include <boost/di.hpp>
#include <type_traits>

#include "cpp_not_null.h"
#include "di_make_injector.h"

namespace vh::di {
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
 * @brief Enables creation of not null pointers for type.
 */
template <typename T>
auto EnableNnPointers [[nodiscard]] () {
  return detail::EnableNnPointers<T>();
}
}  // namespace vh::di

#endif  // VH_DI_ENABLE_NN_POINTERS_H_
