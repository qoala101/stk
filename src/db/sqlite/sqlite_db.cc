#include "sqlite_db.h"

namespace stonks::db {
std::optional<DbError> SqliteDb::CreateTable(
    const TableDefinition &table_definition) {
  return {};
}

std::optional<DbError> SqliteDb::DropTable(const Table &table) { return {}; }

GetTableDefinitionResult SqliteDb::GetTableDefinition(const Table &table) {
  return {};
}

std::optional<DbError> SqliteDb::Insert(const Table &table, const Row &row) {
  return {};
}

std::optional<std::vector<Row>> SqliteDb::Select(const Table &table) {
  return {};
}
}  // namespace stonks::db