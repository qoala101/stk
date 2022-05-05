#ifndef STONKS_DB_NULL_DB_H_
#define STONKS_DB_NULL_DB_H_

#include <optional>
#include <vector>

#include "db.h"

namespace stonks::db {
class NullDb : public Db {
 public:
  static NullDb &Instance();

  bool CreateTable(const TableDefinition &table_definition) override;

  bool DropTable(const Table &table) override;

  bool Insert(const Table &table, const Row &row) override;

  std::optional<std::vector<Row>> Select(
      const TableDefinition &table_definition) override;
};
}  // namespace stonks::db

#endif  // STONKS_DB_NULL_DB_H_
