#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_BINANCE_ENDPOINTS_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_BINANCE_ENDPOINTS_H_

#include "network_typed_endpoint.h"

namespace stonks::app::dt::binance::endpoints {
/**
 * @copydoc Client::GetCurrentAveragePrice
 */
[[nodiscard]] auto GetCurrentAveragePrice() -> const network::TypedEndpoint &;
}  // namespace stonks::app::dt::binance::endpoints

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_BINANCE_ENDPOINTS_H_
