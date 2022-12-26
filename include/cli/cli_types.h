#ifndef STONKS_CLI_CLI_TYPES_H_
#define STONKS_CLI_CLI_TYPES_H_

#include <cstdint>
#include <string>

#include "cpp_variant_struct.h"

namespace stonks::cli {
/**
 * @brief Command line option value.
 */
struct OptionValueVariant
    : public cpp::VariantStruct<int, int64_t, std::string, std::vector<int>,
                                std::vector<int64_t>,
                                std::vector<std::string>> {};
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_TYPES_H_
