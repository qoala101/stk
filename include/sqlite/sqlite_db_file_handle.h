#ifndef STONKS_SQLITE_SQLITE_DB_FILE_HANDLE_H_
#define STONKS_SQLITE_SQLITE_DB_FILE_HANDLE_H_

#include <string>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_copy_const.h"
#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "sqlite_raw_handles.h"

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
  SqliteDbFileHandle(cpp::NnSp<log::ILogger> logger,
                     SqliteDbHandle sqlite_db_handle, std::string file_path);

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

  /**
   * @copydoc GetSqliteDb
   */
  [[nodiscard]] auto GetSqliteDb() -> sqlite3 &;

  /**
   * @brief Gives file path DB was read from and would be stored to
   * when handle is destroyed.
   */
  [[nodiscard]] auto GetFilePath() const -> const std::string &;

 private:
  template <cpp::DecaysTo<SqliteDbFileHandle> This>
  [[nodiscard]] static auto GetSqliteDbImpl(This &t)
      -> cpp::CopyConst<This, sqlite3 &>;

  cpp::NnSp<log::ILogger> logger_;
  SqliteDbHandle sqlite_db_handle_;
  std::string file_path_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_FILE_HANDLE_H_
