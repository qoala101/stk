#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_PDS_APP_CLIENT_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_PDS_APP_CLIENT_H_

#include "app_dt_types.h"
#include "cpp_not_null.h"
#include "network_i_rest_request_sender.h"
#include "network_rest_client.h"

namespace stonks::app::dt::stec {
class PdsAppClient {
 public:
  explicit PdsAppClient(network::RestClient rest_client);

  /**
   * @copydoc pds::App::RecordSymbolPrice
   */
  void RecordSymbolPrice(const SymbolPriceRecord &record);

 private:
  network::RestClient rest_client_;
};
}  // namespace stonks::app::dt::stec

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_PDS_APP_CLIENT_H_
