#ifndef STONKS_APP_SERVICE_SERVICE_CREATE_SERVICE_CLIENT_INJECTOR_H_
#define STONKS_APP_SERVICE_SERVICE_CREATE_SERVICE_CLIENT_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_bind_value_type_to_value.h"
#include "di_make_injector.h"
#include "service_client.h"
#include "service_options.h"

namespace stonks::service {
/**
 * @brief Binds target type to service client implementation.
 */
template <networkx::ClientServerType Target>
auto CreateServiceClientInjector(const ServiceOptions<Target> &options) {
  return di::MakeInjector(
      stonks::di::BindValueTypeToValue(options.GetUri()),
      stonks::di::BindInterfaceToImplementation<Target,
                                                ServiceClient<Target>>());
}
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SERVICE_CREATE_SERVICE_CLIENT_INJECTOR_H_
