#ifndef STONKS_DB_SQLITE_SQLITE_DB_H_
#define STONKS_DB_SQLITE_SQLITE_DB_H_

#include <optional>
#include <string_view>
#include <vector>

#include "db.h"
#include "db_types.h"

class sqlite3;

namespace stonks::db::sqlite {
class SqliteDb : public Db {
 public:
  static std::optional<SqliteDb> OpenOrCreateDbFromFile(
      std::string_view file_name);

  explicit SqliteDb(SqliteDb &&other);
  explicit SqliteDb(const SqliteDb &) = delete;
  SqliteDb &operator=(SqliteDb &&other);
  SqliteDb &operator=(const SqliteDb &) = delete;
  ~SqliteDb() override;

  bool CreateTableIfNotExists(const TableDefinition &table_definition) override;

  bool DropTable(const Table &table) override;

  bool Insert(const Table &table, const Row &row) override;

  bool Delete(const Table &table, std::string_view where) override;

  auto Update(const Table &table, const Row &row, std::string_view where)
      -> bool override;

  std::optional<std::vector<Row>> Select(
      const TableDefinition &table_definition) const override;

  std::optional<std::vector<Row>> Select(
      std::string_view query,
      const std::vector<Column> &columns) const override;

 private:
  friend void swap(SqliteDb &left, SqliteDb &right);

  explicit SqliteDb(sqlite3 *sqlite_db_handle = nullptr);

  sqlite3 *sqlite_db_handle_{};
};
}  // namespace stonks::db::sqlite

#endif  // STONKS_DB_SQLITE_SQLITE_DB_H_
