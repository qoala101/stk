#ifndef STONKS_CPP_CPP_COPY_CONST_H_
#define STONKS_CPP_CPP_COPY_CONST_H_

#include "cpp_concepts.h"  // IWYU pragma: keep

namespace stonks::cpp {
namespace detail {
template <typename From, typename To>
struct CopyConstImpl {
  using T = To;
};

template <Const From, typename To>
struct CopyConstImpl<From, To> {
  using T = const To;
};

template <Const From, Reference To>
struct CopyConstImpl<From, To> {
  using T = const std::remove_reference_t<To> &;
};

template <Const From, Rvalue To>
struct CopyConstImpl<From, To> {
  using T = const std::remove_reference_t<To> &&;
};
}  // namespace detail

/**
 * @brief Applies const specifier to second type if the first one is const.
 */
template <typename From, typename To>
using CopyConst = typename detail::CopyConstImpl<From, To>::T;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_COPY_CONST_H_
