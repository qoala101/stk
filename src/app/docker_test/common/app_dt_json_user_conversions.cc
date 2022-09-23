#include "app_dt_json_user_conversions.h"

#include <cstdint>

#include "network_json_basic_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto ParseFromJson(const IJson &json) -> absl::Time {
  return absl::FromUnixMillis(ParseFromJson<int64_t>(json));
}

auto ConvertToJson(absl::Time value) -> cpp::Pv<IJson> {
  return ConvertToJson(absl::ToUnixMillis(value));
}

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
