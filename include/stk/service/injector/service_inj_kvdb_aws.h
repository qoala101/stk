#ifndef STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_KVDB_AWS_H_
#define STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_KVDB_AWS_H_

#include "aws_dynamodb_sync_db_proxy.h"
#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "kvdb_i_items_interface.h"

namespace stonks::service::inj {
inline auto CreateKvdbAwsInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<kvdb::ITablesInterface,
                                        aws::dynamodb::SyncDbProxy>(),
      di::BindInterfaceToImplementation<kvdb::IItemsInterface,
                                        aws::dynamodb::SyncDbProxy>(),
      di::BindInterfaceToImplementation<kvdb::IDb,
                                        aws::dynamodb::SyncDbProxy>());
}
}  // namespace stonks::service::inj

#endif  // STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_KVDB_AWS_H_
