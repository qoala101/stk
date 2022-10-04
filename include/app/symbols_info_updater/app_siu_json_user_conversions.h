#ifndef STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_JSON_USER_CONVERSIONS_H_
#define STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_JSON_USER_CONVERSIONS_H_

#include "app_siu_types.h"
#include "network_i_json.h"
#include "network_json_basic_conversions.h"  // IWYU pragma: keep

namespace stonks::network {
template <>
[[nodiscard]] auto ParseFromJson(const IJson &json)
    -> app::siu::BinanceSymbolExchangeInfo;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json)
    -> std::vector<app::siu::BinanceSymbolExchangeInfo>;
}  // namespace stonks::network

#endif  // STONKS_APP_SYMBOLS_INFO_UPDATER_APP_SIU_JSON_USER_CONVERSIONS_H_
