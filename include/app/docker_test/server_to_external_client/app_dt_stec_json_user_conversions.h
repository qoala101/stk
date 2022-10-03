#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_JSON_USER_CONVERSIONS_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_JSON_USER_CONVERSIONS_H_

#include "app_dt_stec_binance_types.h"
#include "network_i_json.h"

namespace stonks::network {
template <>
[[nodiscard]] auto ParseFromJson(const IJson &json)
    -> app::dt::stec::binance::AvgPrice;
}  // namespace stonks::network

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_JSON_USER_CONVERSIONS_H_
