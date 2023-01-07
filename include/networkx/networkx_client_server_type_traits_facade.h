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

  /**
   * @brief Gives client-server name.
   */
  static constexpr auto GetName [[nodiscard]] () {
    if constexpr (requires { TargetTraits::kName; }) {
      return TargetTraits::kName;
    } else {
      return "";
    }
  }

  /**
   * @brief Gives default server host.
   */
  static constexpr auto GetDefaultHost [[nodiscard]] () {
    if constexpr (requires { TargetTraits::kDefaultHost; }) {
      return TargetTraits::kDefaultHost;
    } else {
      return "0.0.0.0";
    }
  }

  /**
   * @brief Gives default server port.
   */
  static constexpr auto GetDefaultPort [[nodiscard]] () {
    if constexpr (requires { TargetTraits::kDefaultPort; }) {
      return TargetTraits::kDefaultPort;
    } else {
      return "80";
    }
  }

  /**
   * @brief Gives default synchronization setting for server.
   * @remark Can be overriden for each endpoint function.
   */
  static constexpr auto IsSynchronized [[nodiscard]] () {
    if constexpr (requires { TargetTraits::kSynchronized; }) {
      return TargetTraits::kSynchronized;
    } else {
      return false;
    }
  }
};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_CLIENT_SERVER_TYPE_TRAITS_FACADE_H_
