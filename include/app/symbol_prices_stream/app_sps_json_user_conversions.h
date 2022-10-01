#ifndef STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_JSON_USER_CONVERSIONS_H_
#define STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_JSON_USER_CONVERSIONS_H_

#include "app_sps_types.h"
#include "network_i_json.h"

namespace stonks::network {
template <typename T>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> T;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json)
    -> app::sps::BinanceSymbolBookTick;
}  // namespace stonks::network

#endif  // STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_JSON_USER_CONVERSIONS_H_
