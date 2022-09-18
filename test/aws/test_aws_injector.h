#ifndef STONKS_AWS_TEST_AWS_INJECTOR_H_
#define STONKS_AWS_TEST_AWS_INJECTOR_H_

#include "aws_dynamodb_sync_db_proxy.h"
#include "cpp_di_bind_interface_to_implementation.h"
#include "cpp_di_make_injector.h"
#include "nosqldb_i_items_interface.h"
#include "nosqldb_i_tables_interface.h"

namespace test::aws {
[[nodiscard]] inline auto Injector() -> auto& {
  static auto injector = stonks::cpp::di::MakeInjector(
      stonks::cpp::di::BindInterfaceToImplementation<
          stonks::nosqldb::ITablesInterface,
          stonks::aws::dynamodb::SyncDbProxy>(),
      stonks::cpp::di::BindInterfaceToImplementation<
          stonks::nosqldb::IItemsInterface,
          stonks::aws::dynamodb::SyncDbProxy>());
  return injector;
}
}  // namespace test::aws

#endif  // STONKS_AWS_TEST_AWS_INJECTOR_H_
