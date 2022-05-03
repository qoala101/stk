#ifndef STONKS_DB_DB_H_
#define STONKS_DB_DB_H_

#include "db_types.h"

namespace stonks::db {
class Db {
 public:
  virtual std::optional<DbError> CreateTable(
      const TableDefinition &table_definition) = 0;

  virtual std::optional<DbError> DropTable(const Table &table) = 0;

  virtual GetTableDefinitionResult GetTableDefinition(const Table &table) = 0;

  virtual std::optional<DbError> Insert(const Table &table, const Row &row) = 0;

  virtual std::optional<std::vector<Row>> Select(const Table &table) = 0;
};
}  // namespace stonks::db

#endif  // STONKS_DB_DB_H_
