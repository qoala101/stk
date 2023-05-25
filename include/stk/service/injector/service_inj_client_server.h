/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_SERVICE_INJ_CLIENT_SERVER_H_
#define VH_STK_SERVICE_INJ_CLIENT_SERVER_H_

#include <type_traits>

#include "di_bind_interface_to_implementation.h"
#include "di_bind_value_type_to_value.h"
#include "di_enable_nn_pointers.h"
#include "di_make_injector.h"
#include "networkx_uri.h"
#include "service_client_base.h"
#include "service_client_options.h"
#include "service_server_options.h"

namespace vh::stk::service::inj {
namespace detail {
template <typename Implementation, networkx::ClientServerType Target>
  requires std::is_base_of_v<Target, Implementation>
auto CreateClientServerInjector(networkx::Uri<Target> uri) {
  auto uri_injector = di::BindValueTypeToValue(std::move(uri));

  if constexpr (cpp::Interface<Target>) {
    return di::MakeInjector(
        std::move(uri_injector),
        di::BindInterfaceToImplementation<Target, Implementation>());
  } else {
    return di::MakeInjector(std::move(uri_injector),
                            di::EnableNnPointers<Implementation>());
  }
}
}  // namespace detail

/**
 * @brief Binds target type to service client implementation.
 */
template <typename Client, networkx::ClientServerType Target>
  requires std::is_base_of_v<ClientBase<Target>, Client>
auto CreateClientInjector(const ClientOptions<Target> &options) {
  return detail::CreateClientServerInjector<Client>(options.GetUri());
}

/**
 * @brief Binds target type to service server implementation.
 */
template <typename Server, networkx::ClientServerType Target>
  requires std::is_base_of_v<Target, Server>
auto CreateServerInjector(const ServerOptions<Target> &options) {
  return detail::CreateClientServerInjector<Server>(options.GetUri());
}
}  // namespace vh::stk::service::inj

#endif  // VH_STK_SERVICE_INJ_CLIENT_SERVER_H_
