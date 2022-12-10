#ifndef STONKS_CLI_CLI_CONCEPTS_H_
#define STONKS_CLI_CLI_CONCEPTS_H_

#include "cli_types.h"
#include "cpp_concepts.h"

namespace stonks::cli {
/**
 * @brief Supported command line option types.
 */
template <typename T>
concept SupportedOptionType = cpp::VariantOf<T, OptionValueVariant::ValueType>;
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_CONCEPTS_H_
