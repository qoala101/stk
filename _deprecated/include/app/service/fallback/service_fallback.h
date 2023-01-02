#ifndef STONKS_APP_SERVICE_FALLBACK_SERVICE_FALLBACK_H_
#define STONKS_APP_SERVICE_FALLBACK_SERVICE_FALLBACK_H_

#include <cppcoro/task.hpp>

namespace stonks::service {
/**
 * @brief Gives REST clients errors for bad requests.
 */
class Fallback {
 public:
  /**
   * @brief Fallback for URI which is not exposed by the app.
   */
  auto HandleNotSupportedUri() const -> cppcoro::task<>;
};
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_FALLBACK_SERVICE_FALLBACK_H_
