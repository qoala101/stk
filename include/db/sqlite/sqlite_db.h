#ifndef STONKS_DB_SQLITE_SQLITE_DB_H_
#define STONKS_DB_SQLITE_SQLITE_DB_H_

#include <memory>
#include <string_view>

#include "db.h"
#include "db_prepared_statement.h"
#include "db_row.h"
#include "db_types.h"

namespace stonks::db::sqlite {
class SqliteDb : public Db, public std::enable_shared_from_this<SqliteDb> {
 public:
  /**
   * @brief Creates in memory Sqlite DB.
   * @remark DB can be saved to file via SqliteDb::Backup.
   */
  explicit SqliteDb() = default;

  /**
   * @brief Opens or creates new Sqlite DB from file.
   * @remark File is created if it doesn't exist.
   */
  explicit SqliteDb(std::string_view file_name);

  SqliteDb(const SqliteDb &) = delete;
  SqliteDb(SqliteDb &&) = default;

  auto operator=(const SqliteDb &) -> SqliteDb & = delete;
  auto operator=(SqliteDb &&) -> SqliteDb & = default;

  /**
   * @brief Does backup to the file specified at the time of creation and closes
   * DB handle.
   * @remark All prepared statements become invalid after this.
   */
  ~SqliteDb() override;

  /**
   * @copydoc Db::CreateTableIfNotExists
   */
  void CreateTableIfNotExists(const TableDefinition &table_definition) override;

  /**
   * @copydoc Db::DropTable
   */
  void DropTable(const Table &table) override;

  /**
   * @copydoc Db::Delete
   */
  void DeleteFrom(const Table &table, std::string_view where_clause) override;

  /**
   * @copydoc Db::UpdateRow
   */
  void UpdateRow(const Table &table, const Row &new_row_values,
                 std::string_view where_clause) override;

  /**
   * @copydoc Db::PrepareStatement
   */
  [[nodiscard]] auto PrepareStatement(std::string_view query)
      -> std::unique_ptr<PreparedStatement> override;

  /**
   * @brief Stores DB to the file specified at the creation time.
   */
  void Backup() const;

  /**
   * @brief Stores DB to the specified file.
   */
  void Backup(std::string_view file_name) const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::db::sqlite

#endif  // STONKS_DB_SQLITE_SQLITE_DB_H_
