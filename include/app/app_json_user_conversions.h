#ifndef STONKS_APP_APP_JSON_USER_CONVERSIONS_H_
#define STONKS_APP_APP_JSON_USER_CONVERSIONS_H_

#include <absl/time/time.h>

#include "core_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
#include "network_json_basic_conversions.h"  // IWYU pragma: keep

namespace stonks::network {
template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> absl::Time;
[[nodiscard]] auto ConvertToJson(absl::Time value) -> cpp::Pv<IJson>;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> core::AssetInfo;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> core::SymbolInfo;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> core::SymbolPriceRecord;
}  // namespace stonks::network

namespace stonks::core {
[[nodiscard]] auto ConvertToJson(const AssetInfo &value)
    -> cpp::Pv<network::IJson>;

[[nodiscard]] auto ConvertToJson(const SymbolInfo &value)
    -> cpp::Pv<network::IJson>;

[[nodiscard]] auto ConvertToJson(const SymbolPriceRecord &value)
    -> cpp::Pv<network::IJson>;
}  // namespace stonks::core

#endif  // STONKS_APP_APP_JSON_USER_CONVERSIONS_H_
