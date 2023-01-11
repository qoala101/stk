#include "sqldb_ts_update_statement.h"

namespace stonks::sqldb::ts {
UpdateStatement::UpdateStatement(cpp::NnUp<IUpdateStatement> statement)
    : statement_{std::move(statement)} {}

void UpdateStatement::Execute(std::vector<Value> params) const {
  return statement_->Execute(std::move(params));
}
}  // namespace stonks::sqldb::ts