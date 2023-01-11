#ifndef STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_SELECT_STATEMENT_H_
#define STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_SELECT_STATEMENT_H_

#include <vector>

#include "sqldb_i_select_statement.h"
#include "sqldb_rows.h"
#include "sqldb_value.h"

namespace stonks::sqldb::ts {
/**
 * @brief Statement wrapper which adds synchronization layer.
 */
class SelectStatement : public ISelectStatement {
 public:
  /**
   * @param statement Statement to wrap.
   */
  explicit SelectStatement(cpp::NnUp<ISelectStatement> statement);

  /**
   * @copydoc ISelectStatement::Execute
   */
  auto Execute [[nodiscard]] (std::vector<Value> params) const -> Rows override;

 private:
  cpp::NnUp<ISelectStatement> statement_;
};
}  // namespace stonks::sqldb::ts

#endif  // STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_SELECT_STATEMENT_H_
