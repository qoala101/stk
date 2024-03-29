/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "service_json_conversions.h"

#include <absl/time/time.h>

#include <cstdint>

#include "core_types.h"
#include "network_json_base_conversions.h"
#include "network_json_conversions_facades.h"

namespace vh::network {
template <>
auto JsonParser<absl::Duration>::operator()(const IJson &json) const -> Type {
  return absl::Milliseconds(ParseFromJson<int64_t>(json));
}

auto ConvertToJson(absl::Duration value) -> cpp::Pv<IJson> {
  return ConvertToJson(absl::ToInt64Milliseconds(value));
}

template <>
auto JsonParser<absl::Time>::operator()(const IJson &json) const -> Type {
  return absl::FromUnixMillis(ParseFromJson<int64_t>(json));
}

auto ConvertToJson(absl::Time value) -> cpp::Pv<IJson> {
  return ConvertToJson(absl::ToUnixMillis(value));
}

template <>
auto JsonParser<stk::core::AssetInfo>::operator()(const IJson &json) const
    -> Type {
  return MakeFromJson<Type>(json, "asset", "min_amount", "price_step");
}

auto ConvertToJson(const stk::core::AssetInfo &value)
    -> cpp::Pv<network::IJson> {
  return network::BuildJsonFrom("asset", value.asset, "min_amount",
                                value.min_amount, "price_step",
                                value.price_step);
}

template <>
auto JsonParser<stk::core::SymbolInfo>::operator()(const IJson &json) const
    -> Type {
  return MakeFromJson<Type>(json, "symbol", "base_asset", "quote_asset");
}

auto ConvertToJson(const stk::core::SymbolInfo &value)
    -> cpp::Pv<network::IJson> {
  return network::BuildJsonFrom("symbol", value.symbol, "base_asset",
                                value.base_asset, "quote_asset",
                                value.quote_asset);
}

template <>
auto JsonParser<stk::core::SymbolPriceRecord>::operator()(
    const IJson &json) const -> Type {
  return MakeFromJson<Type>(json, "symbol", "buy_price", "sell_price", "time");
}

auto ConvertToJson(const stk::core::SymbolPriceRecord &value)
    -> cpp::Pv<network::IJson> {
  return network::BuildJsonFrom("symbol", value.symbol, "buy_price",
                                value.buy_price, "sell_price", value.sell_price,
                                "time", value.time);
}
}  // namespace vh::network