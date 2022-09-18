#ifndef STONKS_APP_APP_NETWORK_RESTSDK_INJECTOR_H_
#define STONKS_APP_APP_NETWORK_RESTSDK_INJECTOR_H_

#include "cpp_di_bind_interface_to_implementation.h"
#include "cpp_di_make_injector.h"
#include "network_i_rest_request_receiver.h"
#include "network_i_rest_request_sender.h"
#include "restsdk_rest_request_receiver.h"
#include "restsdk_rest_request_sender.h"

namespace stonks::app::injectors {
[[nodiscard]] inline auto MakeNetworkRestsdkInjector() {
  return cpp::di::MakeInjector(
      cpp::di::BindInterfaceToImplementation<network::IRestRequestReceiver,
                                             restsdk::RestRequestReceiver>(),
      cpp::di::BindInterfaceToImplementation<network::IRestRequestSender,
                                             restsdk::RestRequestSender>());
}
}  // namespace stonks::app::injectors

#endif  // STONKS_APP_APP_NETWORK_RESTSDK_INJECTOR_H_
