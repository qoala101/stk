#include "cli_options.h"

#include <CLI/App.hpp>
#include <CLI/Option.hpp>
#include <gsl/assert>
#include <memory>
#include <type_traits>
#include <variant>

#include "cpp_typed_struct.h"

namespace stonks::cli::detail {
void OptionsBase::AddAsNativeOptions(CLI::App &app) const {
  for (const auto &[name, value] : option_values_) {
    app.add_option(name);
  }
}

void OptionsBase::SetValuesFromNativeOptions(const CLI::App &app) const {
  for (const auto &[name, weak_value] : option_values_) {
    const auto *native_option = app.get_option(name);
    Expects(native_option != nullptr);

    if (native_option->empty()) {
      continue;
    }

    auto value = weak_value.lock();

    if (value == nullptr) {
      continue;
    }

    std::visit(
        [native_option](auto &v) {
          using V = std::remove_cvref_t<decltype(v)>;

          v = native_option->as<V>();
        },
        **value);
  }
}
}  // namespace stonks::cli::detail