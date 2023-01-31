#ifndef VH_STK_SERVICE_SERVER_OPTIONS_H_
#define VH_STK_SERVICE_SERVER_OPTIONS_H_

#include <string>
#include <string_view>

#include "cli_option.h"
#include "cli_options.h"
#include "network_types.h"
#include "networkx_client_server_type_traits_facade.h"
#include "networkx_concepts.h"
#include "networkx_uri.h"

namespace vh::stk::service {
namespace detail {
class ServerOptionsBase {
 protected:
  ServerOptionsBase(cli::Options &options, std::string_view name,
                    std::string_view default_port);

  auto GetUri [[nodiscard]] () const -> network::Uri;

 private:
  cli::Option<std::string> port;
};
}  // namespace detail

/**
 * @brief Command line options required to run a target server.
 */
template <networkx::ClientServerType Target>
class ServerOptions : public detail::ServerOptionsBase {
 private:
  using TargetTraits = networkx::ClientServerTypeTraitsFacade<Target>;

 public:
  /**
   * @brief Adds required command line options.
   */
  explicit ServerOptions(cli::Options &options)
      : ServerOptionsBase{options, TargetTraits::GetName(),
                          TargetTraits::GetDefaultPort()} {}

  /**
   * @brief Parses options and builds server URI from it.
   */
  auto GetUri [[nodiscard]] () const {
    return networkx::Uri<Target>{ServerOptionsBase::GetUri()};
  }
};
}  // namespace vh::stk::service

#endif  // VH_STK_SERVICE_SERVER_OPTIONS_H_
