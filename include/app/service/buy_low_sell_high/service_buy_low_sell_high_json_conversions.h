#ifndef STONKS_APP_SERVICE_BUY_LOW_SELL_HIGH_SERVICE_BUY_LOW_SELL_HIGH_JSON_CONVERSIONS_H_
#define STONKS_APP_SERVICE_BUY_LOW_SELL_HIGH_SERVICE_BUY_LOW_SELL_HIGH_JSON_CONVERSIONS_H_

#include "core_blsh_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace stonks::network {
auto ConvertToJson [[nodiscard]] (const core::blsh::Operation &value)
-> cpp::Pv<IJson>;
}  // namespace stonks::network

#endif  // STONKS_APP_SERVICE_BUY_LOW_SELL_HIGH_SERVICE_BUY_LOW_SELL_HIGH_JSON_CONVERSIONS_H_
