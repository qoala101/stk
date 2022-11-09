#ifndef STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_HANDLE_H_
#define STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_HANDLE_H_

#include "cpp_not_null.h"
#include "sqlite_db_handle_variant.h"
#include "sqlite_raw_handles.h"

namespace stonks::sqlite {
/**
 * @brief Handle for SQLite prepared statement.
 * @remark Keeps DB alive while handle is alive.
 */
class PreparedStatementHandle {
 public:
  PreparedStatementHandle(cpp::NnSp<SqliteDbHandleVariant> sqlite_db_handle,
                          SqliteStatementHandle sqlite_statement_handle);

  auto GetSqliteStatement [[nodiscard]] () const -> sqlite3_stmt &;

 private:
  cpp::NnSp<SqliteDbHandleVariant> sqlite_db_handle_;
  SqliteStatementHandle sqlite_statement_handle_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_HANDLE_H_
