/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CLI_APP_H_
#define VH_CLI_APP_H_

#include "cli_options.h"
#include "cli_run_scope.h"
#include "cpp_concepts.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"

namespace vh::cli {
/**
 * @brief Command line application util.
 * Provides access to command line options, logs its lifetime,
 * and handles interrupts.
 */
class App {
 public:
  /**
   * @brief Parses command line arguments.
   */
  App(int argc, const char *const *argv);

  /**
   * @brief Parses command line arguments and sets option values.
   */
  App(int argc, const char *const *argv, const Options &options);

  /**
   * @brief Runs the function until interrupted.
   */
  template <cpp::VoidCallable Main>
  void Run(const Main &main) const {
    const auto run_scope = CreateRunScope();
    main();
  }

  /**
   * @brief Runs the main function until interrupted.
   * @tparam Main Callable which returns an instance which would be kept alive
   * until interrupt occurs.
   */
  template <cpp::NonVoidCallable Main>
  void Run(const Main &main) const {
    // vh: Optional is used so that instance is destroyed after the run_scope.
    auto instance = cpp::Opt<decltype(main())>{};
    const auto run_scope = CreateRunScope();
    // cppcheck-suppress redundantInitialization
    instance = main();
  }

 private:
  auto CreateRunScope [[nodiscard]] () const -> RunScope;

  cpp::NnSp<CLI::App> app_;
};
}  // namespace vh::cli

#endif  // VH_CLI_APP_H_
