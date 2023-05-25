/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_VARIANT_STRUCT_H_
#define VH_CPP_VARIANT_STRUCT_H_

#include <variant>

#include "cpp_typed_struct.h"

namespace vh::cpp {
/**
 * @brief Base for variant-like types.
 */
template <typename... Ts>
using VariantStruct = TypedStruct<std::variant<Ts...>>;
}  // namespace vh::cpp

#endif  // VH_CPP_VARIANT_STRUCT_H_
