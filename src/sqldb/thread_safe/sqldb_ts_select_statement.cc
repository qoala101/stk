#include "sqldb_ts_select_statement.h"

namespace stonks::sqldb::ts {
SelectStatement::SelectStatement(cpp::NnUp<ISelectStatement> statement)
    : statement_{std::move(statement)} {}

auto SelectStatement::Execute(std::vector<Value> params) const -> Rows {
  return statement_->Execute(std::move(params));
}
}  // namespace stonks::sqldb::ts