#ifndef STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SIU_ENDPOINTS_H_
#define STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SIU_ENDPOINTS_H_

#include "network_typed_endpoint.h"

namespace stonks::core::siu::endpoints {
auto BinanceExchangeInfo [[nodiscard]] () -> network::TypedEndpoint;
}  // namespace stonks::core::siu::endpoints

#endif  // STONKS_APP_CORE_SYMBOLS_INFO_UPDATER_CORE_SIU_ENDPOINTS_H_
