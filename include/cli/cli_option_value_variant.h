#ifndef STONKS_CLI_CLI_OPTION_VALUE_VARIANT_H_
#define STONKS_CLI_CLI_OPTION_VALUE_VARIANT_H_

#include <cstdint>
#include <string>

#include "cpp_variant_struct.h"

namespace CLI {
class App;
class Option;
}  // namespace CLI

namespace stonks::cli {
/**
 * @brief Command line option value.
 */
struct OptionValueVariant
    : public cpp::VariantStruct<int, int64_t, std::string, std::vector<int>,
                                std::vector<int64_t>,
                                std::vector<std::string>> {
  /**
   * @brief Adds option of current type to the native app.
   */
  void AddOptionToNativeApp(std::string option_name, CLI::App &app) const;

  /**
   * @brief Sets current value from native option.
   */
  void SetValueFromNativeOption(const CLI::Option &option);
};
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_OPTION_VALUE_VARIANT_H_
