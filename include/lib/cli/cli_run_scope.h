/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CLI_RUN_SCOPE_H_
#define VH_CLI_RUN_SCOPE_H_

#include "cpp_not_null.h"

namespace CLI {
class App;
}  // namespace CLI

namespace vh::cli {
/**
 * @brief Used to wrap main application function around.
 * Preforms tasks such as logging before and after main application
 * code execution.
 */
class RunScope {
 public:
  explicit RunScope(cpp::NnSp<CLI::App> app);

  RunScope(const RunScope &) = delete;
  RunScope(RunScope &&) noexcept = delete;

  auto operator=(const RunScope &) -> RunScope & = delete;
  auto operator=(RunScope &&) noexcept -> RunScope & = delete;

  /**
   * @brief Blocks current thread until interrupted.
   */
  ~RunScope();

 private:
  cpp::NnSp<CLI::App> app_;
};
}  // namespace vh::cli

#endif  // VH_CLI_RUN_SCOPE_H_
