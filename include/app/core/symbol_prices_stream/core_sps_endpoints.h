#ifndef STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_ENDPOINTS_H_
#define STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_ENDPOINTS_H_

#include "core_types.h"
#include "network_typed_ws_endpoint.h"

namespace stonks::core::sps::endpoints {
/**
 * @brief Gives endpoint for the specified symbol.
 */
auto BinanceSymbolBookTickerStream [[nodiscard]] (Symbol symbol)
-> network::TypedWsEndpoint;
}  // namespace stonks::core::sps::endpoints

#endif  // STONKS_APP_CORE_SYMBOL_PRICES_STREAM_CORE_SPS_ENDPOINTS_H_
