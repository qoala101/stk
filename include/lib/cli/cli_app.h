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
    auto instance = cpp::Opt<decltype(main())>{};
    const auto run_scope = CreateRunScope();
    instance = main();
  }

 private:
  auto CreateRunScope [[nodiscard]] () const -> RunScope;

  cpp::NnSp<CLI::App> app_;
};
}  // namespace vh::cli

#endif  // VH_CLI_APP_H_
