#include "sqlite_prepared_statement_handle.h"

#include <sqlite3.h>

#include <utility>

#include "sqlite_db_handle_variant.h"

namespace stonks::sqlite {
PreparedStatementHandle::PreparedStatementHandle(
    cpp::NnSp<SqliteDbHandleVariant> sqlite_db_handle,
    SqliteStatementHandle sqlite_statement_handle)
    : sqlite_db_handle_{std::move(sqlite_db_handle)},
      sqlite_statement_handle_{std::move(sqlite_statement_handle)} {}

auto PreparedStatementHandle::GetSqliteStatement() const -> sqlite3_stmt& {
  return *sqlite_statement_handle_.get();
}
}  // namespace stonks::sqlite