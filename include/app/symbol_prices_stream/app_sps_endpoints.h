#ifndef STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_ENDPOINTS_H_
#define STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_ENDPOINTS_H_

#include "core_types.h"
#include "network_typed_ws_endpoint.h"

namespace stonks::app::sps::endpoints {
[[nodiscard]] auto BinanceSymbolBookTickerStream(core::Symbol symbol)
    -> network::TypedWsEndpoint;
}  // namespace stonks::app::sps::endpoints

#endif  // STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_ENDPOINTS_H_
