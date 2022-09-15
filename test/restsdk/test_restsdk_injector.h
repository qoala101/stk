#ifndef STONKS_RESTSDK_TEST_RESTSDK_INJECTOR_H_
#define STONKS_RESTSDK_TEST_RESTSDK_INJECTOR_H_

#include <boost/di.hpp>

#include "cpp_di_enable_not_null.h"
#include "cpp_di_factory.h"
#include "log_i_logger.h"
#include "network_i_rest_request_sender.h"
#include "restsdk_rest_request_receiver.h"
#include "restsdk_rest_request_sender.h"
#include "spdlog_logger.h"

namespace test::restsdk {
[[nodiscard]] inline auto Injector() -> auto& {
  // clang-format off
  static auto injector = make_injector(
    boost::di::bind<stonks::network::IRestRequestSender>().to<stonks::restsdk::RestRequestSender>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::network::IRestRequestSender>>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::network::IRestRequestSender>>(),
    boost::di::bind<stonks::network::IRestRequestReceiver>().to<stonks::restsdk::RestRequestReceiver>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::network::IRestRequestReceiver>>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::network::IRestRequestReceiver>>(),
    boost::di::bind<stonks::cpp::di::IFactory<stonks::network::IRestRequestReceiver>>().to(stonks::cpp::di::Factory<stonks::restsdk::RestRequestReceiver>{}),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::cpp::di::IFactory<stonks::network::IRestRequestReceiver>>>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::cpp::di::IFactory<stonks::network::IRestRequestReceiver>>>(),
    boost::di::bind<stonks::log::ILogger>().to<stonks::spdlog::Logger>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::log::ILogger>>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::log::ILogger>>()
  );
  // clang-format on
  return injector;
}
}  // namespace test::restsdk

#endif  // STONKS_RESTSDK_TEST_RESTSDK_INJECTOR_H_
