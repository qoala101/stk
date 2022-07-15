#include "sqldb_i_select_statement.h"

#include "sqldb_rows.h"

namespace stonks::sqldb {
auto ISelectStatement::Execute() const -> Rows { return Execute({}); }
}  // namespace stonks::sqldb