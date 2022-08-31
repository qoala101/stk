#ifndef STONKS_CPP_ALIASES_CPP_NOT_NULL_H_
#define STONKS_CPP_ALIASES_CPP_NOT_NULL_H_

#include "cpp_smart_pointers.h"
#include "not_null.hpp"

/**
 * @file Short cpp::not_null aliases.
 */

namespace stonks::cpp {
template <typename... Ts>
using Nn = ::cpp::not_null<Ts...>;

[[nodiscard]] auto AssumeNn(auto &&...args) {
  return ::cpp::assume_not_null(std::forward<decltype(args)>(args)...);
}

[[nodiscard]] auto CheckNn(auto &&...args) {
  return ::cpp::check_not_null(std::forward<decltype(args)>(args)...);
}

template <typename... Ts>
using NnUp = ::cpp::not_null<Up<Ts...>>;

template <typename... Ts>
using NnSp = ::cpp::not_null<Sp<Ts...>>;

template <typename... Ts>
[[nodiscard]] auto MakeNnUp(auto &&...args) {
  return AssumeNn(MakeUp<Ts...>(std::forward<decltype(args)>(args)...));
}

template <typename... Ts>
[[nodiscard]] auto MakeNnSp(auto &&...args) {
  return AssumeNn(MakeSp<Ts...>(std::forward<decltype(args)>(args)...));
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_ALIASES_CPP_NOT_NULL_H_
