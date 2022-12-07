#include "cli_app.h"

#include <CLI/App.hpp>
#include <CLI/Config.hpp>     // IWYU pragma: keep
#include <CLI/Formatter.hpp>  // IWYU pragma: keep
#include <not_null.hpp>

#include "cli_options.h"
#include "cli_run_scope.h"
#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"

namespace stonks::cli {
App::App(int argc, const char* const* argv)
    : app_{cpp::MakeNnSp<CLI::App>()},
      options_{cpp::CallExposedPrivateConstructorOf<Options, App>{}(app_)} {
  app_->parse(argc, argv);
}

auto App::GetOptions() const -> const Options& { return options_; }

auto App::CreateRunScope() const -> RunScope {
  return cpp::CallExposedPrivateConstructorOf<RunScope, App>{}(app_);
}
}  // namespace stonks::cli