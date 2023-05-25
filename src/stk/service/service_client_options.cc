/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "service_client_options.h"

#include <fmt/core.h>

#include <variant>

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