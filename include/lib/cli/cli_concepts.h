/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CLI_CONCEPTS_H_
#define VH_CLI_CONCEPTS_H_

#include "cli_option_value_variant.h"
#include "cpp_concepts.h"

namespace vh::cli {
/**
 * @brief Supported command line option types.
 */
template <typename T>
concept SupportedOptionType = cpp::VariantOf<T, OptionValueVariant::ValueType>;
}  // namespace vh::cli

#endif  // VH_CLI_CONCEPTS_H_
