#ifndef STONKS_CCUTILS_ALIASES_CCUTILS_OPTIONAL_H_
#define STONKS_CCUTILS_ALIASES_CCUTILS_OPTIONAL_H_

#include <optional>

/**
 * @file Short std::optional aliases.
 */

namespace stonks::ccutils {
template <typename... Ts>
using Opt = std::optional<Ts...>;

template <typename... Ts>
[[nodiscard]] auto MakeOpt(auto &&...args) {
  return std::make_optional<Ts...>(std::forward<decltype(args)>(args)...);
}
}  // namespace stonks::ccutils

#endif  // STONKS_CCUTILS_ALIASES_CCUTILS_OPTIONAL_H_
