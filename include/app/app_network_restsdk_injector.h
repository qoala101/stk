#ifndef STONKS_APP_APP_NETWORK_RESTSDK_INJECTOR_H_
#define STONKS_APP_APP_NETWORK_RESTSDK_INJECTOR_H_

#include <boost/di.hpp>

#include "cpp_di_enable_not_null.h"
#include "cpp_di_factory.h"
#include "cpp_smart_pointers.h"
#include "network_i_rest_request_receiver.h"
#include "restsdk_rest_request_receiver.h"

namespace stonks::app::injectors {
[[nodiscard]] inline auto MakeNetworkRestsdkInjector() {
  // clang-format off
  return make_injector(
    boost::di::bind<cpp::di::IFactory<network::IRestRequestReceiver>>().to(cpp::di::Factory<restsdk::RestRequestReceiver>{}),
    cpp::di::EnableNn<cpp::Up<cpp::di::IFactory<network::IRestRequestReceiver>>>(),
    cpp::di::EnableNn<cpp::Sp<cpp::di::IFactory<network::IRestRequestReceiver>>>()
  );
  // clang-format on
}
}  // namespace stonks::app::injectors

#endif  // STONKS_APP_APP_NETWORK_RESTSDK_INJECTOR_H_
