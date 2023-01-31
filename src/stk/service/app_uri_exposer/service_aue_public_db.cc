#include "service_aue_public_db.h"

#include <coroutine>
#include <string>
#include <utility>

#include "kvdb_types.h"

namespace vh::stk::service::aue {
PublicDb::PublicDb(cpp::NnUp<kvdb::IDb> db) : db_{std::move(db)} {}

auto PublicDb::InsertOrUpdateAppUri(network::Uri uri) const -> cppcoro::task<> {
  const auto table = kvdb::Table{"stk"};
  co_await db_->CreateTableIfNotExists(table);
  co_await db_->InsertOrUpdateItem(table, {"app_uri", uri});
}
}  // namespace vh::stk::service::aue