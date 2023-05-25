/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLDB_PRM_SELECT_STATEMENT_H_
#define VH_SQLDB_PRM_SELECT_STATEMENT_H_

#include <vector>

#include "cpp_not_null.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_prm_types.h"
#include "sqldb_rows.h"
#include "sqldb_value.h"

namespace vh::sqldb::prm {
/**
 * @brief Validates parameter types before execution.
 */
class SelectStatement : public ISelectStatement {
 public:
  /**
   * @param params Query params to validate values against.
   */
  SelectStatement(cpp::NnUp<ISelectStatement> statement,
                  std::vector<QueryParam> params);

  /**
   * @copydoc ISelectStatement::Execute
   */
  auto Execute [[nodiscard]] (std::vector<Value> params) const -> Rows override;

 private:
  cpp::NnUp<ISelectStatement> statement_;
  std::vector<QueryParam> params_{};
};
}  // namespace vh::sqldb::prm

#endif  // VH_SQLDB_PRM_SELECT_STATEMENT_H_
