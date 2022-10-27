#include "sqldb_p_update_statement.h"

#include <gsl/assert>
#include <vector>

#include "cpp_not_null.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_p_common.h"

namespace stonks::sqldb::p {
UpdateStatement::UpdateStatement(cpp::NnUp<IUpdateStatement> statement,
                                 std::vector<Param> params)
    : statement_{std::move(statement)}, params_{std::move(params)} {}

void UpdateStatement::Execute(std::vector<sqldb::Value> params) const {
  Expects(AreParamsValid(params, params_));
  statement_->Execute(params);
}
}  // namespace stonks::sqldb::p