#include "null_db.h"

#include <spdlog/spdlog.h>

namespace stonks::db {
NullDb &NullDb::Instance() {
  static auto instance = NullDb{};
  return instance;
}

bool NullDb::CreateTable(const TableDefinition &table_definition) {
  spdlog::error("Not implemented");
  return false;
}

bool NullDb::DropTable(const Table &table) {
  spdlog::error("Not implemented");
  return false;
}

bool NullDb::Insert(const Table &table, const Row &row) {
  spdlog::error("Not implemented");
  return false;
}

std::optional<std::vector<Row>> NullDb::Select(
    const TableDefinition &table_definition) {
  spdlog::error("Not implemented");
  return std::nullopt;
}
}  // namespace stonks::db