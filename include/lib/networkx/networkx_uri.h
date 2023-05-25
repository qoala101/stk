/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORKX_URI_H_
#define VH_NETWORKX_URI_H_

#include <string_view>

#include "cpp_typed_struct.h"
#include "network_types.h"
#include "networkx_client_server_type_traits_facade.h"
#include "networkx_concepts.h"

namespace vh::networkx {
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
  Uri(const network::Uri &uri) : Uri{*uri} {}
};
}  // namespace vh::networkx

#endif  // VH_NETWORKX_URI_H_
