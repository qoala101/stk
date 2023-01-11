#include "sqldb_ts_db.h"

namespace stonks::sqldb::ts {
Db::Db(cpp::NnUp<IDb> db) : db_{std::move(db)} {}

auto Db::PrepareStatement(SelectQuery query) -> cpp::NnUp<ISelectStatement> {
  return db_->PrepareStatement(std::move(query));
}

auto Db::PrepareStatement(Query query) -> cpp::NnUp<IUpdateStatement> {
  return db_->PrepareStatement(std::move(query));
}
}  // namespace stonks::sqldb::ts