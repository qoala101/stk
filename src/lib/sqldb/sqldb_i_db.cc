#include "sqldb_i_db.h"

#include <not_null.hpp>

namespace vh::sqldb {
void IDb::CreateTableIfNotExistsImpl(qb::Create query_builder) {
  PrepareStatement(query_builder.IfNotExists().Build())->Execute();
}
}  // namespace vh::sqldb