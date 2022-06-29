#ifndef STONKS_SQLITE_SQLITE_DB_H_
#define STONKS_SQLITE_SQLITE_DB_H_

#include <memory>
#include <string_view>

#include "sqldb_db.h"
#include "sqldb_row_definition.h"
#include "sqldb_select_statement.h"
#include "sqldb_update_statement.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
/**
 * @copydoc sqldb::IDb
 */
class SqliteDb : public sqldb::IDb {
 public:
  /**
   * @brief Creates wrapper for SQLite DB.
   */
  explicit SqliteDb(SqliteDbHandle sqlite_db);

  SqliteDb(const SqliteDb &) = delete;
  SqliteDb(SqliteDb &&) noexcept = default;

  auto operator=(const SqliteDb &) -> SqliteDb & = delete;
  auto operator=(SqliteDb &&) noexcept -> SqliteDb & = default;

  /**
   * @brief Closes SQLite DB.
   * @remark Doesn't write DB to file. It should be done manually.
   */
  ~SqliteDb() noexcept override;

  /**
   * @copydoc sqldb::IDb::PrepareStatement
   */
  [[nodiscard]] auto PrepareStatement(
      std::string_view query, const sqldb::RowDefinition &result_definition)
      -> std::unique_ptr<sqldb::ISelectStatement> override;

  /**
   * @copydoc sqldb::IDb::PrepareStatement
   */
  [[nodiscard]] auto PrepareStatement(std::string_view query)
      -> std::unique_ptr<sqldb::IUpdateStatement> override;

  /**
   * @copydoc sqldb::IDb::WriteToFile
   */
  void WriteToFile(std::string_view file_path) override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_H_
