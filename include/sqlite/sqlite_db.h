#ifndef STONKS_DB_SQLITE_SQLITE_DB_H_
#define STONKS_DB_SQLITE_SQLITE_DB_H_

#include <sqlite3.h>

#include <gsl/pointers>
#include <memory>
#include <string_view>

#include "db.h"
#include "db_prepared_statement.h"

namespace stonks::db::sqlite {
class SqliteDb : public Db {
 public:
  /**
   * @brief Creates DB wrapper for SQLite DB.
   */
  explicit SqliteDb(gsl::not_null<sqlite3 *> sqlite_db);

  SqliteDb(const SqliteDb &) = delete;
  SqliteDb(SqliteDb &&) noexcept = default;

  auto operator=(const SqliteDb &) -> SqliteDb & = delete;
  auto operator=(SqliteDb &&) noexcept -> SqliteDb & = default;

  /**
   * @brief Closes SQLite DB.
   * @remark Doesn't write DB to file. It should be done manually.
   * @remark All prepared statements become invalid after this.
   */
  ~SqliteDb() override;

  /**
   * @copydoc Db::PrepareStatement
   */
  [[nodiscard]] auto PrepareStatement(std::string_view query)
      -> std::unique_ptr<PreparedStatement> override;

  /**
   * @copydoc Db::WriteToFile
   */
  void WriteToFile(std::string_view file_path) override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::db::sqlite

#endif  // STONKS_DB_SQLITE_SQLITE_DB_H_
