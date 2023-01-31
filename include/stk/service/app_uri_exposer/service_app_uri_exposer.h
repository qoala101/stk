#ifndef VH_STK_SERVICE_APP_URI_EXPOSER_H_
#define VH_STK_SERVICE_APP_URI_EXPOSER_H_

#include <absl/time/time.h>

#include "cpp_timer.h"
#include "service_aue_impl.h"

namespace vh::stk::service {
/**
 * @brief Periodically exposes app URI to outside world.
 */
class AppUriExposer {
 public:
  struct ConstructorArgs {
    aue::Impl exposer;
    absl::Duration expose_uri_interval{};

    /**
     * @brief Time in which to reattempt the action if it
     * fails.
     * Fail could occur if Ngrok or public DB are not available.
     */
    absl::Duration reattempt_interval{};
  };

  explicit AppUriExposer(ConstructorArgs args);

 private:
  cpp::Timer expose_uri_timer_;
};
}  // namespace vh::stk::service

#endif  // VH_STK_SERVICE_APP_URI_EXPOSER_H_
