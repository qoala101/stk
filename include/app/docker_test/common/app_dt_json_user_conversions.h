#ifndef STONKS_APP_DOCKER_TEST_COMMON_APP_DT_JSON_USER_CONVERSIONS_H_
#define STONKS_APP_DOCKER_TEST_COMMON_APP_DT_JSON_USER_CONVERSIONS_H_

#include "app_dt_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace stonks::network {
template <typename T>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> T;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json)
    -> app::dt::SymbolPriceRecord;
}  // namespace stonks::network

namespace stonks::app::dt {
[[nodiscard]] auto ConvertToJson(const SymbolPriceRecord &value)
    -> cpp::Pv<network::IJson>;
}  // namespace stonks::app::dt

#endif  // STONKS_APP_DOCKER_TEST_COMMON_APP_DT_JSON_USER_CONVERSIONS_H_
