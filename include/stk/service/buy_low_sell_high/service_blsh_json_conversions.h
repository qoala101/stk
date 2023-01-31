#ifndef VH_STK_SERVICE_BLSH_JSON_CONVERSIONS_H_
#define VH_STK_SERVICE_BLSH_JSON_CONVERSIONS_H_

#include "core_blsh_types.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace vh::network {
auto ConvertToJson [[nodiscard]] (const stk::core::blsh::Operation &value)
-> cpp::Pv<IJson>;
}  // namespace vh::network

#endif  // VH_STK_SERVICE_BLSH_JSON_CONVERSIONS_H_
