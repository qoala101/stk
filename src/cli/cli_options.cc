#include "cli_options.h"

#include <CLI/App.hpp>
#include <type_traits>
#include <utility>

namespace stonks::cli {
template <typename T>
auto Options::GetOptionOrImpl(std::string name, T &&default_value) const {
  const auto *option = app_->get_option_no_throw(std::move(name));

  if (option == nullptr) {
    return std::forward<T>(default_value);
  }

  return option->as<std::remove_cvref_t<T>>();
}

auto Options::GetOptionOr(std::string name, int default_value) const -> int {
  return GetOptionOrImpl(std::move(name), default_value);
}

auto Options::GetOptionOr(std::string name, int64_t default_value) const
    -> int64_t {
  return GetOptionOrImpl(std::move(name), default_value);
}

auto Options::GetOptionOr(std::string name, std::string default_value) const
    -> std::string {
  return GetOptionOrImpl(std::move(name), std::move(default_value));
}

Options::Options(cpp::NnSp<CLI::App> app) : app_{std::move(app)} {}
}  // namespace stonks::cli