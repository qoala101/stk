#ifndef STONKS_APP_APP_JSON_USER_CONVERSIONS_H_
#define STONKS_APP_APP_JSON_USER_CONVERSIONS_H_

#include <absl/time/time.h>

#include "core_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace stonks::network {
auto ConvertToJson [[nodiscard]] (absl::Time value) -> cpp::Pv<IJson>;
}  // namespace stonks::network

namespace stonks::core {
auto ConvertToJson [[nodiscard]] (const AssetInfo &value)
-> cpp::Pv<network::IJson>;

auto ConvertToJson [[nodiscard]] (const SymbolInfo &value)
-> cpp::Pv<network::IJson>;

auto ConvertToJson [[nodiscard]] (const SymbolPriceRecord &value)
-> cpp::Pv<network::IJson>;
}  // namespace stonks::core

#endif  // STONKS_APP_APP_JSON_USER_CONVERSIONS_H_
