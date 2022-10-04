#ifndef STONKS_APP_DOCKER_TEST_COMMON_APP_DT_JSON_USER_CONVERSIONS_H_
#define STONKS_APP_DOCKER_TEST_COMMON_APP_DT_JSON_USER_CONVERSIONS_H_

#include <absl/time/time.h>

#include "app_dt_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace stonks::network {
[[nodiscard]] auto ConvertToJson(absl::Time value) -> cpp::Pv<IJson>;
}  // namespace stonks::network

namespace stonks::app::dt {
[[nodiscard]] auto ConvertToJson(const SymbolPriceRecord &value)
    -> cpp::Pv<network::IJson>;
}  // namespace stonks::app::dt

#endif  // STONKS_APP_DOCKER_TEST_COMMON_APP_DT_JSON_USER_CONVERSIONS_H_
