#ifndef STONKS_SQLITE_SQLITE_DB_FILE_HANDLE_H_
#define STONKS_SQLITE_SQLITE_DB_FILE_HANDLE_H_

#include "cpp_this.h"  // IWYU pragma: keep
#include "di_factory.h"
#include "log_i_logger.h"
#include "sqlite_raw_handles.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
/**
 * @remark Closes DB connection and writes DB to the file when destroyed.
 */
class SqliteDbFileHandle {
 public:
  /**
   * @param file_path File from which SQLite DB was read
   * and where it's to be written.
   */
  SqliteDbFileHandle(di::Factory<log::ILogger> logger_factory,
                     SqliteDbHandle sqlite_db_handle, FilePath file_path);

  SqliteDbFileHandle(const SqliteDbFileHandle &) = delete;
  SqliteDbFileHandle(SqliteDbFileHandle &&) noexcept = default;

  auto operator=(const SqliteDbFileHandle &) -> SqliteDbFileHandle & = delete;
  auto operator=(SqliteDbFileHandle &&) noexcept
      -> SqliteDbFileHandle & = default;

  /**
   * @brief Closes SQLite DB and writes the changes to file.
   */
  ~SqliteDbFileHandle();

  /**
   * @brief Gives native SQLite handle.
   */
  [[nodiscard]] auto GetSqliteDb() const -> const sqlite3 &;
  [[nodiscard]] auto GetSqliteDb() -> sqlite3 &;

  /**
   * @brief Gives file path DB was read from and would be stored to
   * when handle is destroyed.
   */
  [[nodiscard]] auto GetFilePath() const -> const FilePath &;

 private:
  [[nodiscard]] static auto GetSqliteDbImpl(
      cpp::This<SqliteDbFileHandle> auto &t) -> auto &;

  di::Factory<log::ILogger> logger_factory_;
  SqliteDbHandle sqlite_db_handle_;
  FilePath file_path_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_FILE_HANDLE_H_
