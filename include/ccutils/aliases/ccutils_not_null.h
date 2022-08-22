#ifndef STONKS_CCUTILS_ALIASES_CCUTILS_NOT_NULL_H_
#define STONKS_CCUTILS_ALIASES_CCUTILS_NOT_NULL_H_

#include "ccutils_smart_pointers.h"
#include "not_null.hpp"

/**
 * @file Short cpp::not_null aliases.
 */

namespace stonks::ccutils {
template <typename... Ts>
using Nn = cpp::not_null<Ts...>;

template <typename... Ts>
[[nodiscard]] auto AssumeNn(auto &&...args) {
  return cpp::assume_not_null(std::forward<decltype(args)>(args)...);
}

template <typename... Ts>
using NnUp = cpp::not_null<Up<Ts...>>;

template <typename... Ts>
using NnSp = cpp::not_null<Sp<Ts...>>;

template <typename... Ts>
using NnWp = cpp::not_null<Wp<Ts...>>;

template <typename... Ts>
[[nodiscard]] auto MakeNnUp(auto &&...args) {
  return AssumeNn(MakeUp<Ts...>(std::forward<decltype(args)>(args)...));
}

template <typename... Ts>
[[nodiscard]] auto MakeNnSp(auto &&...args) {
  return AssumeNn(MakeSp<Ts...>(std::forward<decltype(args)>(args)...));
}
}  // namespace stonks::ccutils

#endif  // STONKS_CCUTILS_ALIASES_CCUTILS_NOT_NULL_H_
