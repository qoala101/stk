#include "networkx_uri.h"

#include <fmt/format.h>

namespace stonks::networkx::detail {
UriBase::UriBase(std::string_view uri, std::string_view target_name)
    : Uri{[uri, target_name]() {
        if (target_name.empty()) {
          return std::string{uri};
        }

        return fmt::format("{}/{}", uri, target_name);
      }()} {}
}  // namespace stonks::networkx::detail
