/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_i_db.h"

#include <not_null.hpp>

namespace vh::sqldb {
void IDb::CreateTableIfNotExistsImpl(qb::Create query_builder) {
  PrepareStatement(query_builder.IfNotExists().Build())->Execute();
}
}  // namespace vh::sqldb