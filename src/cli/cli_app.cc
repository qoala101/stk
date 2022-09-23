#include "cli_app.h"

#include <CLI/App.hpp>
#include <CLI/Config.hpp>     // IWYU pragma: keep
#include <CLI/Formatter.hpp>  // IWYU pragma: keep
#include <memory>

#include "cpp_not_null.h"
#include "not_null.hpp"

namespace stonks::cli {
App::App(int argc, const char* const* argv) : app_{cpp::MakeNnSp<CLI::App>()} {
  app_->parse(argc, argv);
}
}  // namespace stonks::cli