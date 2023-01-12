#include "sqldb_ts_update_statement.h"

namespace stonks::sqldb::ts {
void UpdateStatement::Execute(std::vector<Value> params) const {
  const auto lock = std::lock_guard{GetMutex()};
  return GetStatement().Execute(std::move(params));
}
}  // namespace stonks::sqldb::ts