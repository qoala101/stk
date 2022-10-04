#ifndef STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_ENDPOINTS_H_
#define STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_ENDPOINTS_H_

#include "network_typed_endpoint.h"

namespace stonks::app::siu::endpoints {
[[nodiscard]] auto BinanceExchangeInfo() -> const network::TypedEndpoint &;
}  // namespace stonks::app::siu::endpoints

#endif  // STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_ENDPOINTS_H_
