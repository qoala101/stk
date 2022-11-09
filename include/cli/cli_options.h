#ifndef STONKS_CLI_CLI_OPTIONS_H_
#define STONKS_CLI_CLI_OPTIONS_H_

#include <stdint.h>

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
  auto GetOptionOr [[nodiscard]] (std::string name, int default_value) const
      -> int;
  auto GetOptionOr [[nodiscard]] (std::string name, int64_t default_value) const
      -> int64_t;
  auto GetOptionOr
      [[nodiscard]] (std::string name, std::string default_value) const
      -> std::string;

 private:
  friend class cpp::ExposePrivateConstructorsTo<App, Options>;

  explicit Options(cpp::NnSp<CLI::App> app);

  auto GetOptionOrImpl
      [[nodiscard]] (std::string name, auto &&default_value) const;

  cpp::NnSp<CLI::App> app_;
};
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_OPTIONS_H_
