/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_i_select_statement.h"

#include "sqldb_rows.h"

namespace vh::sqldb {
auto ISelectStatement::Execute() const -> Rows { return Execute({}); }
}  // namespace vh::sqldb