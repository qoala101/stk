/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "networkx_uri.h"

#include <fmt/core.h>

#include <string>

namespace vh::networkx::detail {
UriBase::UriBase(std::string_view uri, std::string_view target_name)
    : Uri{[uri, target_name]() {
        if (target_name.empty()) {
          return std::string{uri};
        }

        return fmt::format("{}/{}", uri, target_name);
      }()} {}
}  // namespace vh::networkx::detail
