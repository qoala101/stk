#ifndef STONKS_APP_COMMON_COMMON_JSON_CONVERSIONS_H_
#define STONKS_APP_COMMON_COMMON_JSON_CONVERSIONS_H_

#include <absl/time/time.h>

#include "core_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace stonks::network {
auto ConvertToJson [[nodiscard]] (absl::Time value) -> cpp::Pv<IJson>;
auto ConvertToJson [[nodiscard]] (const core::AssetInfo &value)
-> cpp::Pv<IJson>;
auto ConvertToJson [[nodiscard]] (const core::SymbolInfo &value)
-> cpp::Pv<IJson>;
auto ConvertToJson [[nodiscard]] (const core::SymbolPriceRecord &value)
-> cpp::Pv<IJson>;
}  // namespace stonks::network

#endif  // STONKS_APP_COMMON_COMMON_JSON_CONVERSIONS_H_
