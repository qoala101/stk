#include "networkx_endpoint_function_traits_facade.h"

#include <fmt/core.h>

#include "network_types.h"

namespace stonks::networkx::detail {
auto ToUri(std::string_view function_name) -> network::Uri {
  return {fmt::format("/{}", function_name)};
}
}  // namespace stonks::networkx::detail