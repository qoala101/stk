#ifndef STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_H_
#define STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_H_

#include <sqlite3.h>

#include "ccutils_not_null.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
/**
 * @brief Handle for SQLite prepared statement.
 * @remark Keeps DB alive while handle is alive.
 */
class PreparedStatementHandle {
 public:
  explicit PreparedStatementHandle(
      ccutils::NnSp<sqlite3> sqlite_db_handle,
      SqliteStatementHandle sqlite_statement_handle);

  PreparedStatementHandle(const PreparedStatementHandle &) = delete;
  PreparedStatementHandle(PreparedStatementHandle &&) noexcept = default;

  auto operator=(const PreparedStatementHandle &)
      -> PreparedStatementHandle & = delete;
  auto operator=(PreparedStatementHandle &&) noexcept
      -> PreparedStatementHandle & = default;

  ~PreparedStatementHandle() noexcept = default;

  [[nodiscard]] auto GetSqliteStatement() const -> sqlite3_stmt &;

 private:
  ccutils::NnSp<sqlite3> sqlite_db_handle_;
  SqliteStatementHandle sqlite_statement_handle_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_H_
