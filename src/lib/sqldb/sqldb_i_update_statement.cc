/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_i_update_statement.h"

namespace vh::sqldb {
void IUpdateStatement::Execute() const { Execute({}); }
}  // namespace vh::sqldb