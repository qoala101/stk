#ifndef STONKS_SQLITE_SQLITE_SELECT_STATEMENT_H_
#define STONKS_SQLITE_SQLITE_SELECT_STATEMENT_H_

#include <vector>

#include "sqldb_enums.h"
#include "sqldb_row_definition.h"
#include "sqldb_rows.h"
#include "sqldb_select_statement.h"
#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqlite_prepared_statement_handle.h"

namespace stonks::sqlite {
/**
 * @copydoc sqldb::ISelectStatement
 */
class SqliteSelectStatement : public sqldb::ISelectStatement {
 public:
  /**
   * @param prepared_statement_handle Handle for SQLite prepared statement
   * produced by parent DB.
   * @param result_definition Description of rows to be parsed from prepared
   * statement result.
   */
  explicit SqliteSelectStatement(
      SqlitePreparedStatementHandle &&prepared_statement_handle,
      const sqldb::RowDefinition &result_definition);

  /**
   * @copydoc sqldb::ISelectStatement::Execute
   */
  [[nodiscard]] auto Execute(const std::vector<sqldb::Value> &params)
      -> sqldb::Rows override;

 private:
  SqlitePreparedStatementHandle prepared_statement_handle_;
  std::vector<sqldb::Column> result_columns_{};
  std::vector<sqldb::DataType> result_types_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_SELECT_STATEMENT_H_
