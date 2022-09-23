#include "cli_run_scope.h"

#include <fmt/core.h>

#include <CLI/App.hpp>
#include <memory>
#include <utility>

#include "cli_wait_for_interrupt.h"
#include "cpp_not_null.h"
#include "not_null.hpp"

namespace stonks::cli {
RunScope::RunScope(cpp::NnSp<CLI::App> app) : app_{std::move(app)} {
  fmt::print(R"(Started: {}
)",
             app_->get_name());
}

RunScope::~RunScope() {
  WaitForInterrupt();
  fmt::print(R"(Stopped: {}
)",
             app_->get_name());
}
}  // namespace stonks::cli