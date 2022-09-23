#ifndef STONKS_CLI_CLI_APP_H_
#define STONKS_CLI_CLI_APP_H_

#include "cli_options.h"
#include "cli_run_scope.h"
#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"

namespace stonks::cli {
/**
 * @brief Command line application util.
 * Provides access to command line options, logs its lifetime
 * and handles interrupts.
 */
class App {
 public:
  /**
   * @brief Parses command line arguments.
   */
  App(int argc, const char *const *argv);

  /**
   * @brief Runs the application until it's interrupted
   * @tparam Main Function to execute inside application. Has access to
   * application options and must produce an instance.
   */
  template <std::invocable<const Options &> Main>
  void Run(const Main &main) {
    const auto run_scope =
        cpp::CallExposedPrivateConstructorOf<RunScope, App>{}(app_);
    const auto options =
        cpp::CallExposedPrivateConstructorOf<Options, App>{}(app_);
    const auto instance = main(options);
  }

 private:
  cpp::NnSp<CLI::App> app_;
};
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_APP_H_
