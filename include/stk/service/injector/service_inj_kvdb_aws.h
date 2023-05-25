/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_SERVICE_INJ_KVDB_AWS_H_
#define VH_STK_SERVICE_INJ_KVDB_AWS_H_

#include "aws_dynamodb_sync_db_proxy.h"
#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "kvdb_i_items_interface.h"

namespace vh::stk::service::inj {
inline auto CreateKvdbAwsInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<kvdb::ITablesInterface,
                                        aws::dynamodb::SyncDbProxy>(),
      di::BindInterfaceToImplementation<kvdb::IItemsInterface,
                                        aws::dynamodb::SyncDbProxy>(),
      di::BindInterfaceToImplementation<kvdb::IDb,
                                        aws::dynamodb::SyncDbProxy>());
}
}  // namespace vh::stk::service::inj

#endif  // VH_STK_SERVICE_INJ_KVDB_AWS_H_
