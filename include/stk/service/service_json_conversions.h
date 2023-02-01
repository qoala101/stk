#ifndef VH_STK_SERVICE_JSON_CONVERSIONS_H_
#define VH_STK_SERVICE_JSON_CONVERSIONS_H_

#include <absl/time/time.h>

#include "core_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace vh::network {
auto ConvertToJson [[nodiscard]] (absl::Duration value) -> cpp::Pv<IJson>;
auto ConvertToJson [[nodiscard]] (absl::Time value) -> cpp::Pv<IJson>;
auto ConvertToJson [[nodiscard]] (const stk::core::AssetInfo &value)
-> cpp::Pv<IJson>;
auto ConvertToJson [[nodiscard]] (const stk::core::SymbolInfo &value)
-> cpp::Pv<IJson>;
auto ConvertToJson [[nodiscard]] (const stk::core::SymbolPriceRecord &value)
-> cpp::Pv<IJson>;
}  // namespace vh::network

#endif  // VH_STK_SERVICE_JSON_CONVERSIONS_H_
