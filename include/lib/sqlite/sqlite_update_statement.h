/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLITE_UPDATE_STATEMENT_H_
#define VH_SQLITE_UPDATE_STATEMENT_H_

#include <vector>

#include "sqldb_i_update_statement.h"
#include "sqldb_value.h"
#include "sqlite_prepared_statement_impl.h"

namespace vh::sqlite {
/**
 * @copydoc sqldb::IUpdateStatement
 */
class UpdateStatement : public sqldb::IUpdateStatement {
 public:
  explicit UpdateStatement(PreparedStatementImpl impl);

  /**
   * @copydoc sqldb::IUpdateStatement::Execute
   */
  void Execute(std::vector<sqldb::Value> params) const override;

 private:
  PreparedStatementImpl impl_;
};
}  // namespace vh::sqlite

#endif  // VH_SQLITE_UPDATE_STATEMENT_H_
