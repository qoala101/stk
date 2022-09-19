#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_H_

#include "app_dt_stec_binance_client.h"
#include "app_dt_stec_pds_app_client.h"
#include "app_dt_types.h"

namespace stonks::app::dt::stec {
class App {
 public:
  explicit App(binance::Client binance_client, PdsAppClient pds_app_client);

  [[nodiscard]] auto GetAveragePrice(Symbol symbol) const -> Price;

 private:
  binance::Client binance_client_;
  mutable PdsAppClient pds_app_client_;
};
}  // namespace stonks::app::dt::stec

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_H_
