#ifndef STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_
#define STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_

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

#endif  // STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_
