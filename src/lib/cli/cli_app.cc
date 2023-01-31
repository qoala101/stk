#include "cli_app.h"

#include <CLI/App.hpp>
#include <CLI/Config.hpp>     // IWYU pragma: keep
#include <CLI/Formatter.hpp>  // IWYU pragma: keep
#include <not_null.hpp>

#include "cli_options.h"
#include "cli_run_scope.h"
#include "cpp_not_null.h"

namespace stonks::cli {
App::App(int argc, const char* const* argv) : app_{cpp::MakeNnSp<CLI::App>()} {
  app_->parse(argc, argv);
}

App::App(int argc, const char* const* argv, const Options& options)
    : app_{cpp::MakeNnSp<CLI::App>()} {
  options.AddToNativeApp(*app_);
  app_->parse(argc, argv);
  options.SetValuesFromNativeOptions(*app_);
}

auto App::CreateRunScope() const -> RunScope { return RunScope{app_}; }
}  // namespace stonks::cli