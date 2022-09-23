#include "cli_options.h"

#include <CLI/App.hpp>

namespace stonks::cli {
template <typename T, cpp::DecaysTo<T> U>
auto Options::GetOptionOrImpl(std::string name, U &&default_value) const -> T {
  const auto *option = app_->get_option_no_throw(std::move(name));

  if (option == nullptr) {
    return std::forward<U>(default_value);
  }

  return option->as<T>();
}

auto Options::GetOptionOr(std::string name, int default_value) const -> int {
  return GetOptionOrImpl<int>(std::move(name), default_value);
}

auto Options::GetOptionOr(std::string name, std::string default_value) const
    -> std::string {
  return GetOptionOrImpl<std::string>(std::move(name),
                                      std::move(default_value));
}

Options::Options(cpp::NnSp<CLI::App> app) : app_{std::move(app)} {}
}  // namespace stonks::cli