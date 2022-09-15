#ifndef STONKS_LOG_LOG_FORMAT_H_
#define STONKS_LOG_LOG_FORMAT_H_

#include <fmt/core.h>

namespace stonks::log {
/**
 * @brief fmt::format alias to be used for composing of messages
 * passed to loggers.
 */
template <typename... Args>
[[nodiscard]] auto Format(fmt::format_string<Args...> fmt, Args&&... args) {
  return fmt::format(std::move(fmt), std::forward<Args>(args)...);
}
}  // namespace stonks::log

#endif  // STONKS_LOG_LOG_FORMAT_H_
