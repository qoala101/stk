#ifndef STONKS_DB_SQLITE_SQLITE_PREPARED_STATEMENT_H_
#define STONKS_DB_SQLITE_SQLITE_PREPARED_STATEMENT_H_

#include <sqlite3.h>

#include <functional>
#include <memory>
#include <vector>

#include "sqldb_prepared_statement.h"
#include "sqldb_row_definition.h"
#include "sqldb_rows.h"
#include "sqldb_value.h"

namespace stonks::sqlite {
/**
 * @copydoc PreparedStatement
 */
class SqlitePreparedStatement : public sqldb::PreparedStatement {
 public:
  /**
   * @param statement Statement owned by DB.
   * @param finalize_statement_callback Callback to parent DB which informs it
   * that statement can be finalized.
   */
  explicit SqlitePreparedStatement(
      std::weak_ptr<sqlite3_stmt> statement,
      std::function<void(sqlite3_stmt &)> finalize_statement_callback);

  SqlitePreparedStatement(const SqlitePreparedStatement &) = delete;
  SqlitePreparedStatement(SqlitePreparedStatement &&) noexcept = default;

  auto operator=(const SqlitePreparedStatement &)
      -> SqlitePreparedStatement & = delete;
  auto operator=(SqlitePreparedStatement &&) noexcept
      -> SqlitePreparedStatement & = default;

  /**
   * @brief Lets DB know that statement can be finalized.
   */
  ~SqlitePreparedStatement() override;

  /**
   * @copydoc PreparedStatement::Execute
   */
  [[nodiscard]] auto Execute(const std::vector<sqldb::Value> &params,
                             const sqldb::RowDefinition &result_definition)
      -> sqldb::Rows override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_DB_SQLITE_SQLITE_PREPARED_STATEMENT_H_
