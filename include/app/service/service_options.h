#ifndef STONKS_APP_SERVICE_SERVICE_OPTIONS_H_
#define STONKS_APP_SERVICE_SERVICE_OPTIONS_H_

#include <string>

#include "cli_options.h"
#include "network_types.h"
#include "networkx_client_server_type_traits_facade.h"
#include "networkx_concepts.h"
#include "networkx_uri.h"

namespace stonks::service {
namespace detail {
class ServiceOptionsBase {
 protected:
  ServiceOptionsBase(cli::Options &options, std::string_view name,
                     std::string_view default_host,
                     std::string_view default_port);

  auto GetUri [[nodiscard]] () const -> network::Uri;

 private:
  cli::Option<std::string> host;
  cli::Option<std::string> port;
};
}  // namespace detail

/**
 * @brief Command line options required to connect to service.
 */
template <networkx::ClientServerType Target>
class ServiceOptions : public detail::ServiceOptionsBase {
 private:
  using TargetTraits = networkx::ClientServerTypeTraitsFacade<Target>;

 public:
  /**
   * @brief Adds command line options required to connect to service.
   */
  explicit ServiceOptions(cli::Options &options)
      : ServiceOptionsBase{options, TargetTraits::GetName(),
                           TargetTraits::GetDefaultHost(),
                           TargetTraits::GetDefaultPort()} {}

  /**
   * @brief Parses options and builds service URI from it.
   */
  auto GetUri [[nodiscard]] () const {
    return networkx::Uri<Target>{ServiceOptionsBase::GetUri()};
  }
};
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SERVICE_OPTIONS_H_
