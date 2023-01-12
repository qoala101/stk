#ifndef STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_SELECT_STATEMENT_H_
#define STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_SELECT_STATEMENT_H_

#include <mutex>
#include <vector>

#include "sqldb_i_select_statement.h"
#include "sqldb_rows.h"
#include "sqldb_ts_statement_base.h"
#include "sqldb_value.h"

namespace stonks::sqldb::ts {
/**
 * @copydoc StatementBase
 */
class SelectStatement : public StatementBase<ISelectStatement> {
 public:
  using StatementBase::StatementBase;

  /**
   * @copydoc ISelectStatement::Execute
   */
  auto Execute [[nodiscard]] (std::vector<Value> params) const -> Rows override;
};
}  // namespace stonks::sqldb::ts

#endif  // STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_SELECT_STATEMENT_H_
