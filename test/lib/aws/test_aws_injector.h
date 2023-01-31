#ifndef VH_TEST_AWS_INJECTOR_H_
#define VH_TEST_AWS_INJECTOR_H_

#include "aws_dynamodb_sync_db_proxy.h"
#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "kvdb_i_items_interface.h"
#include "kvdb_i_tables_interface.h"
#include "spdlog_thread_safe_logger.h"

namespace test::aws {
inline auto Injector [[nodiscard]] () -> auto& {
  static auto injector = vh::di::MakeInjector(
      vh::di::BindInterfaceToImplementation<vh::log::ILogger,
                                            vh::spdlog::Logger>(),
      vh::di::BindInterfaceToImplementation<vh::kvdb::ITablesInterface,
                                            vh::aws::dynamodb::SyncDbProxy>(),
      vh::di::BindInterfaceToImplementation<vh::kvdb::IItemsInterface,
                                            vh::aws::dynamodb::SyncDbProxy>());
  return injector;
}
}  // namespace test::aws

#endif  // VH_TEST_AWS_INJECTOR_H_
