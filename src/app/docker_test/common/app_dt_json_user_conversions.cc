#include "app_dt_json_user_conversions.h"

#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto ParseFromJson(const IJson &json) -> app::dt::SymbolPriceRecord {
  return MakeFromJson<app::dt::SymbolPriceRecord>(json, "time", "symbol",
                                                  "price");
}
}  // namespace stonks::network

namespace stonks::app::dt {
auto ConvertToJson(const SymbolPriceRecord &value) -> cpp::Pv<network::IJson> {
  return network::BuildJsonFrom("time", value.time, "symbol", value.symbol,
                                "price", value.price);
}
}  // namespace stonks::app::dt
