#ifndef STONKS_APP_APP_NOSQLDB_AWS_INJECTOR_H_
#define STONKS_APP_APP_NOSQLDB_AWS_INJECTOR_H_

#include <boost/di.hpp>

#include "aws_dynamodb_sync_db_proxy.h"
#include "cpp_di_enable_not_null.h"
#include "cpp_smart_pointers.h"
#include "nosqldb_i_items_interface.h"

namespace stonks::app::injectors {
[[nodiscard]] inline auto MakeNosqldbAwsInjector() {
  // clang-format off
  return make_injector(
    boost::di::bind<nosqldb::IItemsInterface>().to<aws::dynamodb::SyncDbProxy>(),
    cpp::di::EnableNn<cpp::Up<nosqldb::IItemsInterface>>(),
    cpp::di::EnableNn<cpp::Sp<nosqldb::IItemsInterface>>()
  );
  // clang-format on
}
}  // namespace stonks::app::injectors

#endif  // STONKS_APP_APP_NOSQLDB_AWS_INJECTOR_H_
