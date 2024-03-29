/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "cli_option_value_variant.h"

#include <CLI/App.hpp>
#include <CLI/Option.hpp>
#include <CLI/StringTools.hpp>
#include <gsl/assert>
#include <type_traits>
#include <utility>
#include <variant>

#include "cpp_concepts.h"
#include "cpp_typed_struct.h"

namespace vh::cli {
void OptionValueVariant::AddOptionToNativeApp(std::string option_name,
                                              CLI::App &app) const {
  std::visit(
      [&app, &option_name](const auto &v) {
        using V = std::remove_cvref_t<decltype(v)>;

        auto *option = app.add_option(std::move(option_name));

        if constexpr (cpp::Vector<V>) {
          Expects(option != nullptr);
          option->expected(CLI::detail::expected_max_vector_size);
          option->delimiter(',');
        }
      },
      **this);
}

void OptionValueVariant::SetValueFromNativeOption(const CLI::Option &option) {
  std::visit(
      [&option](auto &v) {
        using V = std::remove_cvref_t<decltype(v)>;

        v = option.as<V>();
      },
      **this);
}
}  // namespace vh::cli