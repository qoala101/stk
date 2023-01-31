#ifndef STONKS_NETWORKX_NETWORKX_URI_H_
#define STONKS_NETWORKX_NETWORKX_URI_H_

#include <string_view>

#include "network_types.h"
#include "networkx_client_server_type_traits_facade.h"
#include "networkx_concepts.h"

namespace stonks::networkx {
namespace detail {
struct UriBase : public network::Uri {
 protected:
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  UriBase(std::string_view uri, std::string_view target_name);
};
}  // namespace detail

/**
 * @brief URI of specific type of server.
 */
template <ClientServerType Target>
struct Uri : public detail::UriBase {
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  Uri(std::string_view uri)
      : UriBase{uri, ClientServerTypeTraitsFacade<Target>::GetName()} {}

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  Uri(network::Uri uri) : Uri{*uri} {}
};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_URI_H_
