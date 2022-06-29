#include "sqlite_prepared_statement_handle.h"

#include <sqlite3.h>

#include <gsl/assert>
#include <utility>

namespace stonks::sqlite {
SqlitePreparedStatementHandle::SqlitePreparedStatementHandle(
    std::shared_ptr<sqlite3> sqlite_db_handle,
    SqliteStatementHandle sqlite_statement_handle)
    : sqlite_db_handle_{std::move(sqlite_db_handle)},
      sqlite_statement_handle_{std::move(sqlite_statement_handle)} {
  Ensures(sqlite_db_handle_ != nullptr);
  Ensures(sqlite_statement_handle_ != nullptr);
}

auto SqlitePreparedStatementHandle::GetSqliteStatement() const
    -> sqlite3_stmt& {
  return *sqlite_statement_handle_.get();
}
}  // namespace stonks::sqlite