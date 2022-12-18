#include "core_aue_public_db.h"

#include <coroutine>
#include <string>
#include <utility>

#include "kvdb_types.h"

namespace stonks::core::aue {
PublicDb::PublicDb(cpp::NnUp<kvdb::IDb> db) : db_{std::move(db)} {}

auto PublicDb::InsertOrUpdateAppUri(network::Uri uri) const -> cppcoro::task<> {
  const auto table = kvdb::Table{"stonks"};
  db_->CreateTableIfNotExists(table);
  db_->InsertOrUpdateItem(table, {"app_uri", uri});
  // TODO(vh): Rewrite with co_awaits.
  co_return;
}
}  // namespace stonks::core::aue