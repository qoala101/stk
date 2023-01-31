#include "sqldb_prm_select_statement.h"

#include <gsl/assert>
#include <utility>
#include <vector>

#include "cpp_not_null.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_prm_common.h"

namespace vh::sqldb::prm {
SelectStatement::SelectStatement(cpp::NnUp<ISelectStatement> statement,
                                 std::vector<QueryParam> params)
    : statement_{std::move(statement)}, params_{std::move(params)} {}

auto SelectStatement::Execute(std::vector<Value> params) const -> sqldb::Rows {
  Expects(AreParamsValid(params, params_));
  return statement_->Execute(std::move(params));
}
}  // namespace vh::sqldb::p