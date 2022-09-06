#ifndef STONKS_CPP_ALIASES_CPP_POLYMORPHIC_VALUE_H_
#define STONKS_CPP_ALIASES_CPP_POLYMORPHIC_VALUE_H_

#include <polymorphic_value.h>

#include <utility>

/**
 * @file Short isocpp_p0201::polymorphic_value aliases.
 */

namespace stonks::cpp {
template <typename... Ts>
using Pv = isocpp_p0201::polymorphic_value<Ts...>;

template <typename... Ts, typename... Args>
[[nodiscard]] auto MakePv(Args &&...args) {
  return isocpp_p0201::make_polymorphic_value<Ts...>(
      std::forward<Args>(args)...);
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_ALIASES_CPP_POLYMORPHIC_VALUE_H_
