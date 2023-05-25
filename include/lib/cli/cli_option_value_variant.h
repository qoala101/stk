/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CLI_OPTION_VALUE_VARIANT_H_
#define VH_CLI_OPTION_VALUE_VARIANT_H_

#include <cstdint>
#include <string>
#include <vector>

#include "cpp_variant_struct.h"

namespace CLI {
class App;
class Option;
}  // namespace CLI

namespace vh::cli {
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
}  // namespace vh::cli

#endif  // VH_CLI_OPTION_VALUE_VARIANT_H_
