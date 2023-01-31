#ifndef VH_CPP_OPTIONAL_H_
#define VH_CPP_OPTIONAL_H_

#include <optional>

/**
 * @file Short std::optional aliases.
 */

namespace vh::cpp {
template <typename... Ts>
using Opt = std::optional<Ts...>;

template <typename... Ts, typename... Args>
auto MakeOpt [[nodiscard]] (Args &&...args) {
  return std::make_optional<Ts...>(std::forward<Args>(args)...);
}
}  // namespace vh::cpp

#endif  // VH_CPP_OPTIONAL_H_
