#ifndef STONKS_CLI_CLI_APP_H_
#define STONKS_CLI_CLI_APP_H_

#include <type_traits>

#include "cli_options.h"
#include "cli_run_scope.h"
#include "cpp_concepts.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"

namespace stonks::cli {
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
   * @brief Runs the function until interrupted.
   */
  template <cpp::VoidInvocable Main>
  void Run(const Main &main) const {
    const auto run_scope = CreateRunScope();
    main();
  }

  /**
   * @brief Runs the main function until interrupted.
   * @tparam Main Callable which returns an instance which would be kept alive
   * until interrupt occurs.
   */
  template <cpp::NonVoidInvocable Main>
  void Run(const Main &main) const {
    auto instance = cpp::Opt<decltype(main())>{};
    const auto run_scope = CreateRunScope();
    instance = main();
  }

  /**
   * @brief Gives command line options.
   */
  auto GetOptions [[nodiscard]] () const -> const Options &;

 private:
  auto CreateRunScope [[nodiscard]] () const -> RunScope;

  cpp::NnSp<CLI::App> app_;
  Options options_;
};
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_APP_H_
