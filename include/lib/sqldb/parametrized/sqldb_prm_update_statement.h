#ifndef STONKS_SQLDB_PARAMETRIZED_SQLDB_P_UPDATE_STATEMENT_H_
#define STONKS_SQLDB_PARAMETRIZED_SQLDB_P_UPDATE_STATEMENT_H_

#include <vector>

#include "cpp_not_null.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_prm_types.h"
#include "sqldb_value.h"

namespace stonks::sqldb::prm {
/**
 * @brief Validates parameter types before execution.
 */
class UpdateStatement : public IUpdateStatement {
 public:
  /**
   * @param params Query params to validate values against.
   */
  UpdateStatement(cpp::NnUp<IUpdateStatement> statement,
                  std::vector<QueryParam> params);

  /**
   * @copydoc IUpdateStatement::Execute
   */
  void Execute(std::vector<Value> params) const override;

 private:
  cpp::NnUp<IUpdateStatement> statement_;
  std::vector<QueryParam> params_{};
};
}  // namespace stonks::sqldb::p

#endif  // STONKS_SQLDB_PARAMETRIZED_SQLDB_P_UPDATE_STATEMENT_H_
