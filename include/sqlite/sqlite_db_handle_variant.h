#ifndef STONKS_SQLITE_SQLITE_DB_HANDLE_VARIANT_H_
#define STONKS_SQLITE_SQLITE_DB_HANDLE_VARIANT_H_

#include <sqlite3.h>

#include <string>
#include <variant>

#include "cpp_copy_const.h"
#include "sqlite_db_file_handle.h"
#include "sqlite_raw_handles.h"

namespace stonks::sqlite {
/**
 * @brief Variant of SQLite DB handle which keeps connection alive.
 */
class SqliteDbHandleVariant {
 public:
  explicit SqliteDbHandleVariant(SqliteDbHandle sqlite_db);
  explicit SqliteDbHandleVariant(SqliteDbFileHandle sqlite_db);

  /**
   * @brief Gives native SQLite handle.
   */
  [[nodiscard]] auto GetSqliteDb() const -> const sqlite3 &;

  /**
   * @brief Gives native SQLite handle.
   */
  [[nodiscard]] auto GetSqliteDb() -> sqlite3 &;

  /**
   * @brief Gives DB file path or null if DB is in-memory one.
   */
  [[nodiscard]] auto GetFilePath() const -> const std::string *;

 private:
  [[nodiscard]] static auto GetSqliteDbImpl(
      cpp::DecaysTo<SqliteDbHandleVariant> auto &&t)
      -> cpp::CopyConst<decltype(t), sqlite3 &>;

  std::variant<SqliteDbHandle, SqliteDbFileHandle> sqlite_db_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_HANDLE_VARIANT_H_
