#ifndef STONKS_APP_CORE_APP_URI_EXPOSER_CORE_AUE_IMPL_H_
#define STONKS_APP_CORE_APP_URI_EXPOSER_CORE_AUE_IMPL_H_

#include <cppcoro/task.hpp>

#include "core_aue_public_db.h"
#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "ngrok_api.h"

namespace stonks::core::aue {
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
}  // namespace stonks::core::aue

#endif  // STONKS_APP_CORE_APP_URI_EXPOSER_CORE_AUE_IMPL_H_
