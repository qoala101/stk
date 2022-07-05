#include "sqldb_select_statement.h"

#include "sqldb_rows.h"

namespace stonks::sqldb {
auto ISelectStatement::Execute() -> Rows { return Execute({}); }
}  // namespace stonks::sqldb