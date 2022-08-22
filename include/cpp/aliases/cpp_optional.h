#ifndef STONKS_CPP_ALIASES_CPP_OPTIONAL_H_
#define STONKS_CPP_ALIASES_CPP_OPTIONAL_H_

#include <optional>

/**
 * @file Short std::optional aliases.
 */

namespace stonks::cpp {
template <typename... Ts>
using Opt = std::optional<Ts...>;

template <typename... Ts>
[[nodiscard]] auto MakeOpt(auto &&...args) {
  return std::make_optional<Ts...>(std::forward<decltype(args)>(args)...);
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_ALIASES_CPP_OPTIONAL_H_
