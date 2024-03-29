/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "cli_options.h"

#include <CLI/App.hpp>
#include <CLI/Option.hpp>
#include <gsl/assert>
#include <memory>

namespace vh::cli::detail {
void OptionsBase::AddToNativeApp(CLI::App &app) const {
  for (const auto &[name, weak_value] : option_values_) {
    if (const auto value = weak_value.lock()) {
      value->AddOptionToNativeApp(name, app);
    }
  }
}

void OptionsBase::SetValuesFromNativeOptions(const CLI::App &app) const {
  for (const auto &[name, weak_value] : option_values_) {
    const auto *native_option = app.get_option(name);
    Expects(native_option != nullptr);

    if (native_option->empty()) {
      continue;
    }

    if (const auto value = weak_value.lock()) {
      value->SetValueFromNativeOption(*native_option);
    }
  }
}
}  // namespace vh::cli::detail