#include "sqldb_ts_db.h"

#include <mutex>

#include "cpp_not_null.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_ts_select_statement.h"
#include "sqldb_ts_update_statement.h"

namespace stonks::sqldb::ts {
Db::Db(cpp::NnUp<IDb> db)
    : db_{std::move(db)},
      execute_statement_mutex_{cpp::MakeNnSp<std::mutex>()} {}

auto Db::PrepareStatement(SelectQuery query) -> cpp::NnUp<ISelectStatement> {
  return cpp::MakeNnUp<SelectStatement>(db_->PrepareStatement(std::move(query)),
                                        execute_statement_mutex_);
}

auto Db::PrepareStatement(Query query) -> cpp::NnUp<IUpdateStatement> {
  return cpp::MakeNnUp<UpdateStatement>(db_->PrepareStatement(std::move(query)),
                                        execute_statement_mutex_);
}
}  // namespace stonks::sqldb::ts