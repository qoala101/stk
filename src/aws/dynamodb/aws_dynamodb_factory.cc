#include "aws_dynamodb_factory.h"

#include "aws_api_handle.h"
#include "aws_dynamodb_async_db.h"
#include "aws_dynamodb_sync_db_proxy.h"
#include "cpp_not_null.h"

namespace stonks::aws::dynamodb {
auto Factory::CreateTablesInterface() const
    -> cpp::NnUp<nosqldb::ITablesInterface> {
  return CreateDb();
}

auto Factory::CreateItemsInterface() const
    -> cpp::NnUp<nosqldb::IItemsInterface> {
  return CreateDb();
}

auto Factory::CreateDb() const -> cpp::NnUp<nosqldb::IDb> {
  const auto api_handle = ApiHandle::Instance();
  return cpp::MakeNnUp<SyncDbProxy>(AsyncDb{api_handle});
}
}  // namespace stonks::aws::dynamodb