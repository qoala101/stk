/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORKX_CLIENT_SERVER_TYPE_TRAITS_FACADE_H_
#define VH_NETWORKX_CLIENT_SERVER_TYPE_TRAITS_FACADE_H_

#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_types.h"

namespace vh::networkx {
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
};
}  // namespace vh::networkx

#endif  // VH_NETWORKX_CLIENT_SERVER_TYPE_TRAITS_FACADE_H_
