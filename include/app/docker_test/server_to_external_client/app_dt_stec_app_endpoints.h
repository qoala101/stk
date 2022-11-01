#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_ENDPOINTS_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_ENDPOINTS_H_

#include "network_typed_endpoint.h"

namespace stonks::app::dt::stec::endpoints {
/**
 * @copydoc App::GetAveragePrice
 */
[[nodiscard]] auto GetAveragePrice() -> network::TypedEndpoint;
}  // namespace stonks::app::dt::stec::endpoints

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_APP_ENDPOINTS_H_
