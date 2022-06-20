#ifndef STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_H_
#define STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_H_

#include <sqlite3.h>

#include <functional>
#include <memory>

namespace stonks::sqlite {
/**
 * @brief Weak handle for SQLite prepared statement.
 * @remark DB would keep the statement alive while handle is alive.
 */
class SqlitePreparedStatementHandle {
 public:
  /**
   * @param sqlite_statement Statement owned by DB.
   * @param sqlite_statement_handle_deleted_callback Callback to parent DB
   * which informs it that handle is destroyed and DB can delete the statement.
   */
  explicit SqlitePreparedStatementHandle(
      std::weak_ptr<sqlite3_stmt> sqlite_statement,
      std::function<void(sqlite3_stmt &)>
          sqlite_statement_handle_deleted_callback);

  SqlitePreparedStatementHandle(const SqlitePreparedStatementHandle &) = delete;
  SqlitePreparedStatementHandle(SqlitePreparedStatementHandle &&) noexcept =
      default;

  auto operator=(const SqlitePreparedStatementHandle &)
      -> SqlitePreparedStatementHandle & = delete;
  auto operator=(SqlitePreparedStatementHandle &&) noexcept
      -> SqlitePreparedStatementHandle & = default;

  /**
   * @brief Executes handle deleted callback.
   */
  ~SqlitePreparedStatementHandle();

  /**
   * @brief Get the statement.
   * @return Null if parent DB was destroyed.
   */
  [[nodiscard]] auto GetSqliteStatement() const
      -> std::shared_ptr<sqlite3_stmt>;

  /**
   * @brief Checks whether handle points to the existing statement.
   */
  [[nodiscard]] auto IsValid() const -> bool;

 private:
  std::weak_ptr<sqlite3_stmt> sqlite_statement_{};
  std::function<void(sqlite3_stmt &)>
      sqlite_statement_handle_deleted_callback_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_H_
