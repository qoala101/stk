#ifndef STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_
#define STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_

#include <vector>

#include "sqldb_i_update_statement.h"
#include "sqldb_value.h"
#include "sqlite_ps_common_impl.h"

namespace stonks::sqlite {
/**
 * @copydoc sqldb::IUpdateStatement
 */
class UpdateStatement : public sqldb::IUpdateStatement {
 public:
  explicit UpdateStatement(ps::CommonImpl impl);

  /**
   * @copydoc sqldb::IUpdateStatement::Execute
   */
  void Execute(std::vector<sqldb::Value> params) const override;

 private:
  ps::CommonImpl impl_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_
