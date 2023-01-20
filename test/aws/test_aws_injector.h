#ifndef STONKS_AWS_TEST_AWS_INJECTOR_H_
#define STONKS_AWS_TEST_AWS_INJECTOR_H_

#include "aws_dynamodb_sync_db_proxy.h"
#include "di_bind_interface_to_implementation.h"
#include "di_make_injector.h"
#include "kvdb_i_items_interface.h"
#include "kvdb_i_tables_interface.h"
#include "spdlog_thread_safe_logger.h"

namespace test::aws {
inline auto Injector [[nodiscard]] () -> auto& {
  static auto injector = stonks::di::MakeInjector(
      stonks::di::BindInterfaceToImplementation<stonks::log::ILogger,
                                                stonks::spdlog::Logger>(),
      stonks::di::BindInterfaceToImplementation<
          stonks::kvdb::ITablesInterface, stonks::aws::dynamodb::SyncDbProxy>(),
      stonks::di::BindInterfaceToImplementation<
          stonks::kvdb::IItemsInterface, stonks::aws::dynamodb::SyncDbProxy>());
  return injector;
}
}  // namespace test::aws

#endif  // STONKS_AWS_TEST_AWS_INJECTOR_H_
