#ifndef STONKS_DB_DB_H_
#define STONKS_DB_DB_H_

#include "db_types.h"

namespace stonks::db {
class Db {
 public:
  virtual bool CreateTable(const TableDefinition &table_definition) = 0;

  virtual bool DropTable(const Table &table) = 0;

  virtual bool Insert(const Table &table, const Row &row) = 0;

  virtual std::optional<std::vector<Row>> Select(
      const TableDefinition &table_definition) = 0;

  virtual std::optional<std::vector<Row>> Select(
      std::string_view query, const std::vector<Column> &columns) = 0;
};
}  // namespace stonks::db

#endif  // STONKS_DB_DB_H_
