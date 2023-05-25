/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "cli_run_scope.h"

#include <fmt/core.h>

#include <CLI/App.hpp>
#include <not_null.hpp>
#include <string>
#include <utility>

#include "cli_wait_for_interrupt.h"
#include "cpp_not_null.h"

namespace vh::cli {
RunScope::RunScope(cpp::NnSp<CLI::App> app) : app_{std::move(app)} {
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
}  // namespace vh::cli