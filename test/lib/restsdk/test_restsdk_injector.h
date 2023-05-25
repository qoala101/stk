/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_TEST_RESTSDK_INJECTOR_H_
#define VH_TEST_RESTSDK_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "log_i_logger.h"
#include "network_i_rest_request_sender.h"
#include "network_i_ws_client.h"
#include "restsdk_rest_request_receiver.h"
#include "restsdk_rest_request_sender.h"
#include "restsdk_ws_client.h"
#include "spdlog_thread_safe_logger.h"

namespace test::restsdk {
inline auto Injector [[nodiscard]] () -> auto& {
  static auto injector = vh::di::MakeInjector(
      vh::di::BindInterfaceToImplementation<vh::network::IRestRequestSender,
                                            vh::restsdk::RestRequestSender>(),
      vh::di::BindInterfaceToImplementation<vh::network::IRestRequestReceiver,
                                            vh::restsdk::RestRequestReceiver>(),
      vh::di::BindInterfaceToImplementation<vh::network::IWsClient,
                                            vh::restsdk::WsClient>(),
      vh::di::BindInterfaceToImplementation<vh::log::ILogger,
                                            vh::spdlog::ThreadSafeLogger>());
  return injector;
}
}  // namespace test::restsdk

#endif  // VH_TEST_RESTSDK_INJECTOR_H_
