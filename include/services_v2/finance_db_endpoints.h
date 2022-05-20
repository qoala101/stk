#ifndef STONKS_SERVICES_V2_FINANCE_DB_ENDPOINTS_H_
#define STONKS_SERVICES_V2_FINANCE_DB_ENDPOINTS_H_

#include "client_server_types.h"

namespace stonks::finance {
[[nodiscard]] auto SelectAssetsEndpoint() -> stonks::network::EndpointDesc;
[[nodiscard]] auto SelectSymbolsEndpoint() -> stonks::network::EndpointDesc;
[[nodiscard]] auto InsertSymbolPriceTickEndpoint()
    -> stonks::network::EndpointDesc;
[[nodiscard]] auto SelectSymbolPriceTicksEndpoint()
    -> stonks::network::EndpointDesc;
}  // namespace stonks::finance

#endif  // STONKS_SERVICES_V2_FINANCE_DB_ENDPOINTS_H_
