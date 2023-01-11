#ifndef STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_UPDATE_STATEMENT_H_
#define STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_UPDATE_STATEMENT_H_

#include <vector>

#include "sqldb_i_update_statement.h"
#include "sqldb_value.h"

namespace stonks::sqldb::ts {
/**
 * @brief Statement wrapper which adds synchronization layer.
 */
class UpdateStatement : public IUpdateStatement {
 public:
  /**
   * @param statement Statement to wrap.
   */
  explicit UpdateStatement(cpp::NnUp<IUpdateStatement> statement);

  /**
   * @copydoc ISelectStatement::Execute
   */
  void Execute(std::vector<Value> params) const override;

 private:
  cpp::NnUp<IUpdateStatement> statement_;
};
}  // namespace stonks::sqldb::ts

#endif  // STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_UPDATE_STATEMENT_H_
