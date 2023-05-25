/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_SERVICE_AUE_IMPL_H_
#define VH_STK_SERVICE_AUE_IMPL_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "network_types.h"
#include "ngrok_api.h"
#include "service_aue_public_db.h"

namespace vh::stk::service::aue {
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
  auto ExposeNgrokUriIfChanged [[nodiscard]] () -> cppcoro::task<>;

 private:
  PublicDb public_db_;
  ngrok::NgrokApi ngrok_api_;
  cpp::NnUp<log::ILogger> logger_;
  network::Uri last_exposed_uri_{};
};
}  // namespace vh::stk::service::aue

#endif  // VH_STK_SERVICE_AUE_IMPL_H_
