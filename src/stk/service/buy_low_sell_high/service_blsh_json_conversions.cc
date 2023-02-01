// clang-format off
#include "service_json_conversions.h"
// clang-format on

#include "service_blsh_json_conversions.h"

#include "core_blsh_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
#include "network_json_base_conversions.h"
#include "network_json_conversions_facades.h"

namespace vh::network {
template <>
auto JsonParser<stk::core::blsh::Operation>::operator()(const IJson &json) const
    -> Type {
  return MakeFromJson<Type>(json, "time", "type", "btc_buy_price",
                            "btc_sell_price_waiting_for", "btc_balance",
                            "usd_balance");
}

auto ConvertToJson(const stk::core::blsh::Operation &value) -> cpp::Pv<IJson> {
  return network::BuildJsonFrom(
      "time", value.time, "type", value.type, "btc_buy_price",
      value.btc_buy_price, "btc_sell_price_waiting_for",
      value.btc_sell_price_waiting_for, "btc_balance", value.btc_balance,
      "usd_balance", value.usd_balance);
}
}  // namespace vh::network
