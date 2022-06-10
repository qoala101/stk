#include "db_prepared_statement.h"
#include "db_rows.h"

namespace stonks::db {
void PreparedStatement::Execute() { static_cast<void>(Execute({}, {})); }

void PreparedStatement::Execute(const std::vector<Value> &params) {
  static_cast<void>(Execute(params, {}));
}

auto PreparedStatement::Execute(const RowDefinition &result_definition)
    -> Rows {
  return Execute({}, result_definition);
}
}  // namespace stonks::db