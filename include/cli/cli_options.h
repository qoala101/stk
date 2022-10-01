#ifndef STONKS_CLI_CLI_OPTIONS_H_
#define STONKS_CLI_CLI_OPTIONS_H_

#include <string>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"

namespace CLI {
class App;
}  // namespace CLI

namespace stonks::cli {
class App;

/**
 * @brief API to get command line options.
 */
class Options {
 public:
  /**
   * @brief Get int option or default value if not provided.
   */
  [[nodiscard]] auto GetOptionOr(std::string name, int default_value) const
      -> int;

  /**
   * @brief Get string option or default value if not provided.
   */
  [[nodiscard]] auto GetOptionOr(std::string name,
                                 std::string default_value) const
      -> std::string;

 private:
  friend class cpp::ExposePrivateConstructorsTo<App, Options>;

  explicit Options(cpp::NnSp<CLI::App> app);

  template <typename T, cpp::DecaysTo<T> U>
  [[nodiscard]] auto GetOptionOrImpl(std::string name, U &&default_value) const
      -> T;

  cpp::NnSp<CLI::App> app_;
};
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_OPTIONS_H_