#ifndef STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_H_
#define STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_H_

#include <sqlite3.h>

#include <memory>

#include "sqlite_types.h"

namespace stonks::sqlite {
/**
 * @brief Handle for SQLite prepared statement.
 * @remark Keeps DB alive while handle is alive.
 */
class SqlitePreparedStatementHandle {
 public:
  explicit SqlitePreparedStatementHandle(
      std::shared_ptr<sqlite3> sqlite_db_handle,
      SqliteStatementHandle sqlite_statement_handle);

  SqlitePreparedStatementHandle(const SqlitePreparedStatementHandle &) = delete;
  SqlitePreparedStatementHandle(SqlitePreparedStatementHandle &&) noexcept =
      default;

  auto operator=(const SqlitePreparedStatementHandle &)
      -> SqlitePreparedStatementHandle & = delete;
  auto operator=(SqlitePreparedStatementHandle &&) noexcept
      -> SqlitePreparedStatementHandle & = default;

  ~SqlitePreparedStatementHandle() noexcept = default;

  [[nodiscard]] auto GetSqliteStatement() const -> sqlite3_stmt &;

 private:
  std::shared_ptr<sqlite3> sqlite_db_handle_{};
  SqliteStatementHandle sqlite_statement_handle_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_H_
