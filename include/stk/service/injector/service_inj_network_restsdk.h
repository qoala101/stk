#ifndef VH_STK_SERVICE_INJ_NETWORK_RESTSDK_H_
#define VH_STK_SERVICE_INJ_NETWORK_RESTSDK_H_

#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "network_i_rest_request_receiver.h"
#include "network_i_rest_request_sender.h"
#include "network_i_ws_client.h"
#include "restsdk_rest_request_receiver.h"
#include "restsdk_rest_request_sender.h"
#include "restsdk_ws_client.h"

namespace vh::stk::service::inj {
inline auto CreateNetworkRestsdkInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<network::IRestRequestReceiver,
                                        restsdk::RestRequestReceiver>(),
      di::BindInterfaceToImplementation<network::IRestRequestSender,
                                        restsdk::RestRequestSender>(),
      di::BindInterfaceToImplementation<network::IWsClient,
                                        restsdk::WsClient>());
}
}  // namespace vh::stk::service::inj

#endif  // VH_STK_SERVICE_INJ_NETWORK_RESTSDK_H_
