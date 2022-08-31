#include "aws_dynamodb_factory.h"

#include "aws_api_handle.h"
#include "aws_dynamodb_async_db.h"
#include "aws_dynamodb_sync_db_proxy.h"
#include "cpp_not_null.h"

namespace stonks::aws::dynamodb {
auto Factory::LoadDbFromUri(std::string_view uri) const
    -> cpp::NnUp<nosqldb::IDb> {
  const auto api_handle = ApiHandle::Instance();
  return cpp::MakeNnUp<SyncDbProxy>(cpp::MakeNnUp<AsyncDb>(api_handle),
                                    api_handle);
}
}  // namespace stonks::aws::dynamodb