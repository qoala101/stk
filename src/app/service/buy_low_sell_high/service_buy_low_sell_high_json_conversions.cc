// clang-format off
#include "service_json_conversions.h"
// clang-format on

#include "service_buy_low_sell_high_json_conversions.h"

#include "core_blsh_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
#include "network_json_base_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {

template <>
auto JsonParser<core::blsh::Operation>::operator()(const IJson &json) const
    -> Type {
  return MakeFromJson<Type>(json, "time", "base_asset_balance",
                            "quote_asset_balance", "type");
}

auto ConvertToJson(const core::blsh::Operation &value) -> cpp::Pv<IJson> {
  return network::BuildJsonFrom("time", value.time, "base_asset_balance",
                                value.base_asset_balance, "quote_asset_balance",
                                value.quote_asset_balance, "type", value.type);
}
}  // namespace stonks::network
