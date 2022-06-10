#ifndef STONKS_DB_SQLITE_SQLITE_PREPARED_STATEMENT_H_
#define STONKS_DB_SQLITE_SQLITE_PREPARED_STATEMENT_H_

#include <sqlite3.h>

#include <functional>
#include <memory>
#include <vector>

#include "db_prepared_statement.h"
#include "db_rows.h"
#include "db_row_definition.h"
#include "db_value.h"

namespace stonks::db::sqlite {
/**
 * @copydoc PreparedStatement
 */
class SqlitePreparedStatement : public PreparedStatement {
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
  [[nodiscard]] auto Execute(const std::vector<Value> &params,
                             const RowDefinition &result_definition)
      -> Rows override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::db::sqlite

#endif  // STONKS_DB_SQLITE_SQLITE_PREPARED_STATEMENT_H_
