#ifndef STONKS_RESTSDK_TEST_RESTSDK_INJECTOR_H_
#define STONKS_RESTSDK_TEST_RESTSDK_INJECTOR_H_

#include <boost/di.hpp>
#include "cpp_factory.h"
#include "network_i_factory.h"
#include "restsdk_rest_request_receiver.h"
#include "cpp_di_enable_not_null.h"

namespace test::restsdk {
[[nodiscard]] inline auto Injector() -> auto & {
  // clang-format off
  static auto injector = make_injector(
    boost::di::bind<stonks::network::IRestRequestReceiver>().to<stonks::restsdk::RestRequestReceiver>(),
    boost::di::bind<stonks::cpp::IFactory<stonks::network::IRestRequestReceiver>>().to(stonks::cpp::Factory<stonks::restsdk::RestRequestReceiver>{}),
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::cpp::IFactory<stonks::network::IRestRequestReceiver>>>()
  );
  // clang-format on
  return injector;
}
}  // namespace test::restsdk

#endif  // STONKS_RESTSDK_TEST_RESTSDK_INJECTOR_H_
