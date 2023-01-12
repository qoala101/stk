#include "sqldb_ts_select_statement.h"

#include <mutex>

namespace stonks::sqldb::ts {
auto SelectStatement::Execute(std::vector<Value> params) const -> Rows {
  const auto lock = std::lock_guard{GetMutex()};
  return GetStatement().Execute(std::move(params));
}
}  // namespace stonks::sqldb::ts