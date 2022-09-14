#ifndef STONKS_APP_APP_INJECTORS_H_
#define STONKS_APP_APP_INJECTORS_H_

#include <boost/di.hpp>

#include "aws_dynamodb_sync_db_proxy.h"
#include "cpp_di_enable_not_null.h"
#include "cpp_factory.h"
#include "cpp_smart_pointers.h"
#include "network_i_rest_request_receiver.h"
#include "nosqldb_i_items_interface.h"
#include "restsdk_rest_request_receiver.h"

namespace stonks::app::injectors {
[[nodiscard]] inline auto MakeNetworkInjector() {
  // clang-format off
  return make_injector(
    boost::di::bind<cpp::IFactory<network::IRestRequestReceiver>>().to(cpp::Factory<restsdk::RestRequestReceiver>{}),
    cpp::di::EnableNn<cpp::Sp<cpp::IFactory<network::IRestRequestReceiver>>>()
  );
  // clang-format on
}

[[nodiscard]] inline auto MakeNosqldbInjector() {
  // clang-format off
  return make_injector(
    boost::di::bind<nosqldb::IItemsInterface>().to<aws::dynamodb::SyncDbProxy>(),
    cpp::di::EnableNn<cpp::Up<nosqldb::IItemsInterface>>(),
    cpp::di::EnableNn<cpp::Sp<nosqldb::IItemsInterface>>()
  );
  // clang-format on
}
}  // namespace stonks::app::injectors

#endif  // STONKS_APP_APP_INJECTORS_H_
