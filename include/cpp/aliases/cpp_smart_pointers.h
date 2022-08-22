#ifndef STONKS_CPP_ALIASES_CPP_SMART_POINTERS_H_
#define STONKS_CPP_ALIASES_CPP_SMART_POINTERS_H_

#include <memory>

/**
 * @file Short smart pointer aliases.
 */

namespace stonks::cpp {
template <typename... Ts>
using Up = std::unique_ptr<Ts...>;

template <typename... Ts>
using Sp = std::shared_ptr<Ts...>;

template <typename... Ts>
using Wp = std::weak_ptr<Ts...>;

template <typename... Ts>
[[nodiscard]] auto MakeUp(auto &&...args) {
  return std::make_unique<Ts...>(std::forward<decltype(args)>(args)...);
}

template <typename... Ts>
[[nodiscard]] auto MakeSp(auto &&...args) {
  return std::make_shared<Ts...>(std::forward<decltype(args)>(args)...);
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_ALIASES_CPP_SMART_POINTERS_H_
