#include "app_json_user_conversions.h"

#include <cstdint>

#include "core_types.h"
#include "network_json_base_conversions.h"
#include "network_json_conversions_facades.h"

namespace stonks::network {
template <>
auto JsonParser<absl::Time>::operator()(const IJson &json) const -> Type {
  return absl::FromUnixMillis(ParseFromJson<int64_t>(json));
}

auto ConvertToJson(absl::Time value) -> cpp::Pv<IJson> {
  return ConvertToJson(absl::ToUnixMillis(value));
}

template <>
auto JsonParser<core::AssetInfo>::operator()(const IJson &json) const -> Type {
  return MakeFromJson<Type>(json, "asset", "min_amount", "price_step");
}

template <>
auto JsonParser<core::SymbolInfo>::operator()(const IJson &json) const -> Type {
  return MakeFromJson<Type>(json, "symbol", "base_asset", "quote_asset");
}

template <>
auto JsonParser<core::SymbolPriceRecord>::operator()(const IJson &json) const
    -> Type {
  return MakeFromJson<Type>(json, "symbol", "price", "time");
}
}  // namespace stonks::network

namespace stonks::core {
auto ConvertToJson(const AssetInfo &value) -> cpp::Pv<network::IJson> {
  return network::BuildJsonFrom("asset", value.asset, "min_amount",
                                value.min_amount, "price_step",
                                value.price_step);
}

auto ConvertToJson(const SymbolInfo &value) -> cpp::Pv<network::IJson> {
  return network::BuildJsonFrom("symbol", value.symbol, "base_asset",
                                value.base_asset, "quote_asset",
                                value.quote_asset);
}

auto ConvertToJson(const SymbolPriceRecord &value) -> cpp::Pv<network::IJson> {
  return network::BuildJsonFrom("symbol", value.symbol, "price", value.price,
                                "time", value.time);
}
}  // namespace stonks::core