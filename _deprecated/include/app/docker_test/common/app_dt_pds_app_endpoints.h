#ifndef STONKS_APP_DOCKER_TEST_COMMON_APP_DT_PDS_APP_ENDPOINTS_H_
#define STONKS_APP_DOCKER_TEST_COMMON_APP_DT_PDS_APP_ENDPOINTS_H_

#include "network_typed_endpoint.h"

namespace stonks::app::dt::pds::endpoints {
/**
 * @copydoc App::RecordSymbolPrice
 */
auto RecordSymbolPrice [[nodiscard]] () -> network::TypedEndpoint;
}  // namespace stonks::app::dt::pds::endpoints

#endif  // STONKS_APP_DOCKER_TEST_COMMON_APP_DT_PDS_APP_ENDPOINTS_H_
