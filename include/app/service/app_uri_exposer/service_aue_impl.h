#ifndef STONKS_APP_SERVICE_APP_URI_EXPOSER_SERVICE_AUE_IMPL_H_
#define STONKS_APP_SERVICE_APP_URI_EXPOSER_SERVICE_AUE_IMPL_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "ngrok_api.h"
#include "service_aue_public_db.h"

namespace stonks::service::aue {
/**
 * @brief App URI exposer implementation.
 */
class Impl {
 public:
  /**
   * @param public_db DB to which to write the URI.
   * @param ngrok_api Client to running Ngrok which can provide URI.
   */
  Impl(PublicDb public_db, ngrok::NgrokApi ngrok_api,
       cpp::NnUp<log::ILogger> logger);

  /**
   * @brief Writes URI from Ngrok to public DB as app URI.
   */
  auto ExposeNgrokUri [[nodiscard]] () const -> cppcoro::task<>;

 private:
  PublicDb public_db_;
  ngrok::NgrokApi ngrok_api_;
  cpp::NnUp<log::ILogger> logger_;
};
}  // namespace stonks::service::aue

#endif  // STONKS_APP_SERVICE_APP_URI_EXPOSER_SERVICE_AUE_IMPL_H_
