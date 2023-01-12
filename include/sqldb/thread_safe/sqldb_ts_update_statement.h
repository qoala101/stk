#ifndef STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_UPDATE_STATEMENT_H_
#define STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_UPDATE_STATEMENT_H_

#include <mutex>
#include <vector>

#include "sqldb_i_update_statement.h"
#include "sqldb_ts_statement_base.h"
#include "sqldb_value.h"

namespace stonks::sqldb::ts {
/**
 * @copydoc StatementBase
 */
class UpdateStatement : public StatementBase<IUpdateStatement> {
 public:
  using StatementBase::StatementBase;

  /**
   * @copydoc IUpdateStatement::Execute
   */
  void Execute(std::vector<Value> params) const override;
};
}  // namespace stonks::sqldb::ts

#endif  // STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_UPDATE_STATEMENT_H_
