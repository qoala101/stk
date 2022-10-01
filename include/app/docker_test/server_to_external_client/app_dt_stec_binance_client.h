#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_BINANCE_CLIENT_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_BINANCE_CLIENT_H_

#include "app_dt_stec_binance_types.h"
#include "app_dt_types.h"
#include "di_factory.h"
#include "network_i_rest_request_sender.h"
#include "network_rest_client.h"

namespace stonks::app::dt::stec::binance {
class Client {
 public:
  explicit Client(
      di::Factory<network::IRestRequestSender> request_sender_factory);

  [[nodiscard]] auto GetCurrentAveragePrice(const Symbol &symbol) const
      -> AvgPrice;

 private:
  network::RestClient rest_client_;
};
}  // namespace stonks::app::dt::stec::binance

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_BINANCE_CLIENT_H_
