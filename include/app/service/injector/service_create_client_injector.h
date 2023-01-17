#ifndef STONKS_SERVICE_SERVICE_CREATE_CLIENT_INJECTOR_H_
#define STONKS_SERVICE_SERVICE_CREATE_CLIENT_INJECTOR_H_

#include <type_traits>

#include "di_bind_interface_to_implementation.h"
#include "di_bind_value_type_to_value.h"
#include "di_make_injector.h"
#include "service_client_base.h"
#include "service_client_options.h"

namespace stonks::service {
/**
 * @brief Binds target type to service client implementation.
 */
template <typename Client, networkx::ClientServerType Target>
  requires std::is_base_of_v<ClientBase<Target>, Client>
auto CreateClientInjector(const ClientOptions<Target> &options) {
  return di::MakeInjector(
      stonks::di::BindValueTypeToValue(options.GetUri()),
      stonks::di::BindInterfaceToImplementation<Target, Client>());
}
}  // namespace stonks::service

#endif  // STONKS_SERVICE_SERVICE_CREATE_CLIENT_INJECTOR_H_
