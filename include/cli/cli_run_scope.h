#ifndef STONKS_CLI_CLI_RUN_SCOPE_H_
#define STONKS_CLI_CLI_RUN_SCOPE_H_

#include "cpp_meta_private_to.h"
#include "cpp_not_null.h"

namespace CLI {
class App;
}  // namespace CLI

namespace stonks::cli {
class App;

/**
 * @brief Used to wrap main application function around.
 * Preforms tasks such as logging before and after main application
 * code execution.
 */
class RunScope {
 public:
  RunScope(cpp::meta::PrivateTo<App>, cpp::NnSp<CLI::App> app);

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
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_RUN_SCOPE_H_
