#ifndef STONKS_CCUTILS_ALIASES_CCUTILS_POLYMORPHIC_VALUE_H_
#define STONKS_CCUTILS_ALIASES_CCUTILS_POLYMORPHIC_VALUE_H_

#include <polymorphic_value.h>

#include <utility>

/**
 * @file Short polymorphic value aliases.
 */

namespace stonks::ccutils {
template <typename... Ts>
using Pv = isocpp_p0201::polymorphic_value<Ts...>;

template <typename... Ts>
[[nodiscard]] auto MakePv(auto &&...args) {
  return isocpp_p0201::make_polymorphic_value<Ts...>(
      std::forward<decltype(args)>(args)...);
}
}  // namespace stonks::ccutils

#endif  // STONKS_CCUTILS_ALIASES_CCUTILS_POLYMORPHIC_VALUE_H_
