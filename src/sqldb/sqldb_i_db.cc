#include "sqldb_i_db.h"

namespace stonks::sqldb {
void IDb::CreateTableIfNotExistsImpl(qb::Create query_builder) {
  PrepareStatement(query_builder.IfNotExists().Build())->Execute();
}
}  // namespace stonks::sqldb