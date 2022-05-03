#ifndef STONKS_DB_SQLITE_SQLITE_DB_H_
#define STONKS_DB_SQLITE_SQLITE_DB_H_

#include "db.h"

namespace stonks::db {
class SqliteDb : public Db {
 public:
  std::optional<DbError> CreateTable(
      const TableDefinition &table_definition) override;

  std::optional<DbError> DropTable(const Table &table) override;

  GetTableDefinitionResult GetTableDefinition(const Table &table) override;

  std::optional<DbError> Insert(const Table &table, const Row &row) override;

  std::optional<std::vector<Row>> Select(const Table &table) override;
};
}  // namespace stonks::db

#endif  // STONKS_DB_SQLITE_SQLITE_DB_H_
