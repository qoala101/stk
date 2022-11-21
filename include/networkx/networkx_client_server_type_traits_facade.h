#ifndef STONKS_NETWORKX_NETWORKX_CLIENT_SERVER_TYPE_TRAITS_FACADE_H_
#define STONKS_NETWORKX_NETWORKX_CLIENT_SERVER_TYPE_TRAITS_FACADE_H_

#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_types.h"

namespace stonks::networkx {
/**
 * @brief Convenience API over client-server type traits.
 */
template <ClientServerType Target>
struct ClientServerTypeTraitsFacade {
  using TargetTraits = ClientServerTypeTraits<Target>;
  using EndpointFunctions = decltype(TargetTraits::kEndpointFunctions);

  /**
   * @brief Gives number of endpoint functions.
   */
  static constexpr auto GetNumEndpoints [[nodiscard]] () {
    return std::tuple_size_v<EndpointFunctions>;
  }

  /**
   * @brief Gives endpoint function by index.
   */
  template <unsigned kIndex>
  static constexpr auto GetEndpointFunction [[nodiscard]] () {
    return std::get<kIndex>(TargetTraits::kEndpointFunctions);
  }
};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_CLIENT_SERVER_TYPE_TRAITS_FACADE_H_
