#ifndef STONKS_CPP_ALIASES_CPP_NOT_NULL_H_
#define STONKS_CPP_ALIASES_CPP_NOT_NULL_H_

#include <not_null.hpp>

#include "cpp_smart_pointers.h"

/**
 * @file Short cpp::not_null aliases.
 */

namespace stonks::cpp {
template <typename... Ts>
using Nn = ::cpp::not_null<Ts...>;

template <typename... Args>
[[nodiscard]] auto AssumeNn(Args &&...args) {
  return ::cpp::assume_not_null(std::forward<Args>(args)...);
}

template <typename... Args>
[[nodiscard]] auto CheckNn(Args &&...args) {
  return ::cpp::check_not_null(std::forward<Args>(args)...);
}

template <typename... Ts>
using NnUp = Nn<Up<Ts...>>;

template <typename... Ts>
using NnSp = Nn<Sp<Ts...>>;

template <typename... Ts, typename... Args>
[[nodiscard]] auto MakeNnUp(Args &&...args) {
  return AssumeNn(MakeUp<Ts...>(std::forward<Args>(args)...));
}

template <typename... Ts, typename... Args>
[[nodiscard]] auto MakeNnSp(Args &&...args) {
  return AssumeNn(MakeSp<Ts...>(std::forward<Args>(args)...));
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_ALIASES_CPP_NOT_NULL_H_
