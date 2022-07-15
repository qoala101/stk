#include "sqldb_i_update_statement.h"

namespace stonks::sqldb {
void IUpdateStatement::Execute() { Execute({}); }
}  // namespace stonks::sqldb