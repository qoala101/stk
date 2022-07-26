#ifndef STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_
#define STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_

#include <vector>

#include "sqldb_i_update_statement.h"
#include "sqldb_value.h"
#include "sqlite_prepared_statement_handle.h"

namespace stonks::sqlite {
/**
 * @copydoc sqldb::IUpdateStatement
 */
class UpdateStatement : public sqldb::IUpdateStatement {
 public:
  /**
   * @param prepared_statement_handle Handle for SQLite prepared statement
   * produced by parent DB.
   */
  explicit UpdateStatement(PreparedStatementHandle prepared_statement_handle);

  /**
   * @copydoc sqldb::IUpdateStatement::Execute
   */
  void Execute(std::vector<sqldb::Value> params) const override;

 private:
  PreparedStatementHandle prepared_statement_handle_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_
