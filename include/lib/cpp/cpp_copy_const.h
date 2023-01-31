#ifndef STONKS_CPP_CPP_COPY_CONST_H_
#define STONKS_CPP_CPP_COPY_CONST_H_

#include "cpp_concepts.h"  // IWYU pragma: keep

namespace vh::cpp {
namespace detail {
template <typename From, typename To>
struct CopyConstTraits {
  using Type = To;
};

template <Const From, typename To>
struct CopyConstTraits<From, To> {
  using Type = const To;
};

template <Const From, Reference To>
struct CopyConstTraits<From, To> {
  using Type = const std::remove_reference_t<To> &;
};

template <Const From, Rvalue To>
struct CopyConstTraits<From, To> {
  using Type = const std::remove_reference_t<To> &&;
};
}  // namespace detail

/**
 * @brief Applies const specifier to second type if the first one is const.
 */
template <typename From, typename To>
using CopyConst = typename detail::CopyConstTraits<From, To>::Type;
}  // namespace vh::cpp

#endif  // STONKS_CPP_CPP_COPY_CONST_H_
