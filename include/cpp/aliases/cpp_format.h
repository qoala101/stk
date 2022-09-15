#ifndef STONKS_CPP_ALIASES_CPP_FORMAT_H_
#define STONKS_CPP_ALIASES_CPP_FORMAT_H_

#include <fmt/core.h>

/**
 * @file fmt::format aliases.
 */

namespace stonks::cpp {
template <typename... Args>
[[nodiscard]] auto Format(fmt::format_string<Args...> fmt, Args&&... args) {
  return fmt::format(std::move(fmt), std::forward<Args>(args)...);
}
}  // namespace stonks::cpp

#endif  // STONKS_CPP_ALIASES_CPP_FORMAT_H_
