#ifndef VH_CPP_NOT_NULL_H_
#define VH_CPP_NOT_NULL_H_

#include <not_null.hpp>

#include "cpp_smart_pointers.h"

/**
 * @file Short cpp::not_null aliases.
 */

namespace vh::cpp {
template <typename... Ts>
using Nn = ::cpp::not_null<Ts...>;

template <typename T>
using Nullable = std::remove_cvref_t<decltype(std::declval<T>().as_nullable())>;

template <typename... Args>
auto AssumeNn [[nodiscard]] (Args &&...args) {
  return ::cpp::assume_not_null(std::forward<Args>(args)...);
}

template <typename... Args>
auto CheckNn [[nodiscard]] (Args &&...args) {
  return ::cpp::check_not_null(std::forward<Args>(args)...);
}

template <typename... Ts>
using NnUp = Nn<Up<Ts...>>;

template <typename... Ts>
using NnSp = Nn<Sp<Ts...>>;

template <typename... Ts, typename... Args>
auto MakeNnUp [[nodiscard]] (Args &&...args) {
  return AssumeNn(MakeUp<Ts...>(std::forward<Args>(args)...));
}

template <typename... Ts, typename... Args>
auto MakeNnSp [[nodiscard]] (Args &&...args) {
  return AssumeNn(MakeSp<Ts...>(std::forward<Args>(args)...));
}
}  // namespace vh::cpp

#endif  // VH_CPP_NOT_NULL_H_
