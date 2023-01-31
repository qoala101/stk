#ifndef VH_STK_SERVICE_CLIENT_OPTIONS_H_
#define VH_STK_SERVICE_CLIENT_OPTIONS_H_

#include <string>

#include "cli_options.h"
#include "network_types.h"
#include "networkx_client_server_type_traits_facade.h"
#include "networkx_concepts.h"
#include "networkx_uri.h"

namespace vh::stk::service {
namespace detail {
class ClientOptionsBase {
 protected:
  ClientOptionsBase(cli::Options &options, std::string_view name,
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
class ClientOptions : public detail::ClientOptionsBase {
 private:
  using TargetTraits = networkx::ClientServerTypeTraitsFacade<Target>;

 public:
  /**
   * @brief Adds required command line options.
   */
  explicit ClientOptions(cli::Options &options)
      : ClientOptionsBase{options, TargetTraits::GetName(),
                          TargetTraits::GetDefaultHost(),
                          TargetTraits::GetDefaultPort()} {}

  /**
   * @brief Parses options and builds service URI from it.
   */
  auto GetUri [[nodiscard]] () const {
    return networkx::Uri<Target>{ClientOptionsBase::GetUri()};
  }
};
}  // namespace vh::stk::service

#endif  // VH_STK_SERVICE_CLIENT_OPTIONS_H_
