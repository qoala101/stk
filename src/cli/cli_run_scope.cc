#include "cli_run_scope.h"

#include <fmt/core.h>

#include <CLI/App.hpp>
#include <not_null.hpp>
#include <string>
#include <utility>

#include "cli_wait_for_interrupt.h"
#include "cpp_not_null.h"

namespace stonks::cli {
RunScope::RunScope(cpp::meta::PrivateTo<App> /*unused*/,
                   cpp::NnSp<CLI::App> app)
    : app_{std::move(app)} {
  fmt::print(R"(Starting: {}
)",
             app_->get_name());
}

RunScope::~RunScope() {
  fmt::print(R"(Started. Waiting for interrupt.
)");
  WaitForInterrupt();
  fmt::print(R"(Stopped: {}
)",
             app_->get_name());
}
}  // namespace stonks::cli