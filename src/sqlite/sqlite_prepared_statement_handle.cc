#include "sqlite_prepared_statement_handle.h"

#include <sqlite3.h>

#include <utility>

namespace stonks::sqlite {
SqlitePreparedStatementHandle::SqlitePreparedStatementHandle(
    cpp::not_null<std::shared_ptr<sqlite3>> sqlite_db_handle,
    SqliteStatementHandle sqlite_statement_handle)
    : sqlite_db_handle_{std::move(sqlite_db_handle)},
      sqlite_statement_handle_{std::move(sqlite_statement_handle)} {}

auto SqlitePreparedStatementHandle::GetSqliteStatement() const
    -> sqlite3_stmt& {
  return *sqlite_statement_handle_.get();
}
}  // namespace stonks::sqlite