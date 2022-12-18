#ifndef STONKS_APP_SERVICE_SERVICE_KVDB_AWS_INJECTOR_H_
#define STONKS_APP_SERVICE_SERVICE_KVDB_AWS_INJECTOR_H_

#include "aws_dynamodb_sync_db_proxy.h"
#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "kvdb_i_items_interface.h"

namespace stonks::service::injectors {
inline auto CreateKvdbAwsInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<kvdb::ITablesInterface,
                                        aws::dynamodb::SyncDbProxy>(),
      di::BindInterfaceToImplementation<kvdb::IItemsInterface,
                                        aws::dynamodb::SyncDbProxy>(),
      di::BindInterfaceToImplementation<kvdb::IDb,
                                        aws::dynamodb::SyncDbProxy>());
}
}  // namespace stonks::service::injectors

#endif  // STONKS_APP_SERVICE_SERVICE_KVDB_AWS_INJECTOR_H_
