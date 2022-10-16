#include "cli_app.h"

#include <CLI/App.hpp>
#include <CLI/Config.hpp>     // IWYU pragma: keep
#include <CLI/Formatter.hpp>  // IWYU pragma: keep
#include <memory>
#include <not_null.hpp>

#include "cli_run_scope.h"
#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"

namespace stonks::cli {
App::App(int argc, const char* const* argv) : app_{cpp::MakeNnSp<CLI::App>()} {
  app_->parse(argc, argv);
}

auto App::CreateRunScope() const -> RunScope {
  return cpp::CallExposedPrivateConstructorOf<RunScope, App>{}(app_);
}

auto App::CreateOptions() const -> Options {
  return cpp::CallExposedPrivateConstructorOf<Options, App>{}(app_);
}
}  // namespace stonks::cli