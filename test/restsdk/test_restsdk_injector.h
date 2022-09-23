#ifndef STONKS_RESTSDK_TEST_RESTSDK_INJECTOR_H_
#define STONKS_RESTSDK_TEST_RESTSDK_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "log_i_logger.h"
#include "network_i_rest_request_sender.h"
#include "restsdk_rest_request_receiver.h"
#include "restsdk_rest_request_sender.h"
#include "spdlog_logger.h"

namespace test::restsdk {
[[nodiscard]] inline auto Injector() -> auto& {
  static auto injector = stonks::di::MakeInjector(
      stonks::di::BindInterfaceToImplementation<
          stonks::network::IRestRequestSender,
          stonks::restsdk::RestRequestSender>(),
      stonks::di::BindInterfaceToImplementation<
          stonks::network::IRestRequestReceiver,
          stonks::restsdk::RestRequestReceiver>(),
      stonks::di::BindInterfaceToImplementation<stonks::log::ILogger,
                                                stonks::spdlog::Logger>());
  return injector;
}
}  // namespace test::restsdk

#endif  // STONKS_RESTSDK_TEST_RESTSDK_INJECTOR_H_
