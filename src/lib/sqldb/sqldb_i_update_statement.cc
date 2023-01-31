#include "sqldb_i_update_statement.h"

namespace vh::sqldb {
void IUpdateStatement::Execute() const { Execute({}); }
}  // namespace vh::sqldb