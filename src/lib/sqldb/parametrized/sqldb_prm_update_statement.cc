/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_prm_update_statement.h"

#include <gsl/assert>
#include <utility>
#include <vector>

#include "cpp_not_null.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_prm_common.h"

namespace vh::sqldb::prm {
UpdateStatement::UpdateStatement(cpp::NnUp<IUpdateStatement> statement,
                                 std::vector<QueryParam> params)
    : statement_{std::move(statement)}, params_{std::move(params)} {}

void UpdateStatement::Execute(std::vector<sqldb::Value> params) const {
  Expects(AreParamsValid(params, params_));
  statement_->Execute(params);
}
}  // namespace vh::sqldb::prm