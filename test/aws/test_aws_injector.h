#ifndef STONKS_AWS_TEST_AWS_INJECTOR_H_
#define STONKS_AWS_TEST_AWS_INJECTOR_H_

#include <boost/di.hpp>

#include "aws_dynamodb_sync_db_proxy.h"
#include "cpp_di_enable_not_null.h"
#include "cpp_di_factory.h"
#include "network_i_rest_request_sender.h"
#include "nosqldb_i_items_interface.h"
#include "nosqldb_i_tables_interface.h"
#include "restsdk_rest_request_receiver.h"
#include "restsdk_rest_request_sender.h"

namespace test::aws {
[[nodiscard]] inline auto Injector() -> auto& {
  // clang-format off
  static auto injector = make_injector(
    boost::di::bind<stonks::nosqldb::ITablesInterface>().to<stonks::aws::dynamodb::SyncDbProxy>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::nosqldb::ITablesInterface>>(),
    boost::di::bind<stonks::nosqldb::IItemsInterface>().to<stonks::aws::dynamodb::SyncDbProxy>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::nosqldb::IItemsInterface>>()
  );
  // clang-format on
  return injector;
}
}  // namespace test::aws

#endif  // STONKS_AWS_TEST_AWS_INJECTOR_H_
