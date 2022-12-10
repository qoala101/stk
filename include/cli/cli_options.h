#ifndef STONKS_CLI_CLI_OPTIONS_H_
#define STONKS_CLI_CLI_OPTIONS_H_

#include <map>
#include <string>
#include <utility>

#include "cli_concepts.h"
#include "cli_option.h"
#include "cli_types.h"
#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"

namespace CLI {
class App;
}  // namespace CLI

namespace stonks::cli {
namespace detail {
class OptionsBase {
 public:
  void AddAsNativeOptions(CLI::App &app) const;

  void SetValuesFromNativeOptions(const CLI::App &app) const;

 protected:
  void StoreOptionValue(std::string &option_name,
                        const cpp::NnSp<OptionValueVariant> &default_value) {
    option_values_.emplace(std::move(option_name), default_value.as_nullable());
  }

 private:
  std::map<std::string, cpp::Wp<OptionValueVariant>> option_values_{};
};
}  // namespace detail

/**
 * @brief API to get command line options.
 * Options are parsed when constructing the app.
 */
class Options : public detail::OptionsBase {
 public:
  /**
   * @brief Adds option with default value.
   * The actual value can be retrieved after constructing the app.
   */
  template <SupportedOptionType T>
  auto AddOption [[nodiscard]] (std::string name, T default_value)
  -> Option<T> {
    auto option_value = cpp::MakeNnSp<OptionValueVariant>(
        OptionValueVariant{std::forward<T>(default_value)});
    StoreOptionValue(name, option_value);
    return Option<T>{std::move(option_value)};
  }

  auto AddOption [[nodiscard]] (std::string name, const char *default_value) {
    return AddOption(std::move(name), std::string{default_value});
  }
};
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_OPTIONS_H_
