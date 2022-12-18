#ifndef STONKS_APP_CORE_APP_URI_EXPOSER_CORE_APP_URI_EXPOSER_H_
#define STONKS_APP_CORE_APP_URI_EXPOSER_CORE_APP_URI_EXPOSER_H_

#include <absl/time/time.h>

#include "core_aue_impl.h"
#include "cpp_timer.h"

namespace stonks::core {
/**
 * @brief Periodically exposes app URI to outside world.
 */
class AppUriExposer {
 public:
  /**
   * @param reattempt_interval Time in which to reattempt the action if it
   * fails.
   * Fail could occur if Ngrok or public DB are not available.
   */
  AppUriExposer(absl::Duration expose_uri_interval, aue::Impl exposer,
                absl::Duration reattempt_interval);

 private:
  cpp::Timer expose_uri_timer_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_APP_URI_EXPOSER_CORE_APP_URI_EXPOSER_H_
