#ifndef STONKS_APP_APP_NOSQLDB_AWS_INJECTOR_H_
#define STONKS_APP_APP_NOSQLDB_AWS_INJECTOR_H_

#include "aws_dynamodb_sync_db_proxy.h"
#include "cpp_di_bind_interface_to_implementation.h"
#include "cpp_di_make_injector.h"
#include "nosqldb_i_items_interface.h"

namespace stonks::app::injectors {
[[nodiscard]] inline auto MakeNosqldbAwsInjector() {
  return cpp::di::MakeInjector(
      cpp::di::BindInterfaceToImplementation<nosqldb::ITablesInterface,
                                             aws::dynamodb::SyncDbProxy>(),
      cpp::di::BindInterfaceToImplementation<nosqldb::IItemsInterface,
                                             aws::dynamodb::SyncDbProxy>(),
      cpp::di::BindInterfaceToImplementation<nosqldb::IDb,
                                             aws::dynamodb::SyncDbProxy>());
}
}  // namespace stonks::app::injectors

#endif  // STONKS_APP_APP_NOSQLDB_AWS_INJECTOR_H_
