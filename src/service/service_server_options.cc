#include "service_server_options.h"

#include <fmt/format.h>

namespace stonks::service::detail {
ServerOptionsBase::ServerOptionsBase(cli::Options &options,
                                     std::string_view name,
                                     std::string_view default_port)
    : port{options.AddOption(fmt::format("--{}_port", name),
                             default_port.data())} {}

auto ServerOptionsBase::GetUri() const -> network::Uri {
  return {fmt::format("http://0.0.0.0:{}", *port)};
}
}  // namespace stonks::service::detail