#ifndef STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_
#define STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_

#include <vector>

#include "sqldb_update_statement.h"
#include "sqldb_value.h"
#include "sqlite_prepared_statement_handle.h"

namespace stonks::sqlite {
/**
 * @copydoc sqldb::IUpdateStatement
 */
class SqliteUpdateStatement : public sqldb::IUpdateStatement {
 public:
  /**
   * @param prepared_statement_handle Handle for SQLite prepared statement
   * produced by parent DB.
   */
  explicit SqliteUpdateStatement(
      SqlitePreparedStatementHandle prepared_statement_handle);

  /**
   * @copydoc sqldb::IUpdateStatement::Execute
   */
  void Execute(const std::vector<sqldb::Value> &params) override;

 private:
  SqlitePreparedStatementHandle prepared_statement_handle_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_UPDATE_STATEMENT_H_
