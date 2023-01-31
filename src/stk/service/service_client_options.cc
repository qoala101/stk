#include "service_client_options.h"

#include <fmt/format.h>

namespace vh::stk::service::detail {
ClientOptionsBase::ClientOptionsBase(cli::Options &options,
                                     std::string_view name,
                                     std::string_view default_host,
                                     std::string_view default_port)
    : host{options.AddOption(fmt::format("--{}_host", name),
                             default_host.data())},
      port{options.AddOption(fmt::format("--{}_port", name),
                             default_port.data())} {}

auto ClientOptionsBase::GetUri() const -> network::Uri {
  return {fmt::format("http://{}:{}", *host, *port)};
}
}  // namespace vh::stk::service::detail