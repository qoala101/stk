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

template <typename... Ts, typename... Args>
[[nodiscard]] auto MakeUp(Args &&...args) {
  return std::make_unique<Ts...>(std::forward<Args>(args)...);
}

template <typename... Ts, typename... Args>
[[nodiscard]] auto MakeSp(Args &&...args) {
  return std::make_shared<Ts...>(std::forward<Args>(args)...);
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_ALIASES_CPP_SMART_POINTERS_H_
