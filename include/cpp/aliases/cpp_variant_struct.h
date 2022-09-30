#ifndef STONKS_CPP_ALIASES_CPP_VARIANT_STRUCT_H_
#define STONKS_CPP_ALIASES_CPP_VARIANT_STRUCT_H_

#include <variant>

#include "cpp_typed_struct.h"

namespace stonks::cpp {
/**
 * @brief Base for variant-like types.
 */
template <typename... Ts>
using VariantStruct = TypedStruct<std::variant<Ts...>>;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_ALIASES_CPP_VARIANT_STRUCT_H_
