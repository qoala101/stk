#ifndef STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_FACADE_H_
#define STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_FACADE_H_

#include <vector>

#include "cpp_not_null.h"
#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqlite_types.h"

class sqlite3_stmt;

namespace stonks::sqlite {
/**
 * @brief Convenience API for SQLite prepared statement.
 */
class PreparedStatementFacade {
 public:
  explicit PreparedStatementFacade(cpp::Nn<sqlite3_stmt *> sqlite_statement);

  /**
   * @brief Resets prepared statement.
   * @remark Should be called before new parameters binding.
   */
  void Reset() const;

  /**
   * @brief Binds parameters in the order they defined in the statement.
   */
  void BindParams(const std::vector<sqldb::Value> &params) const;

  /**
   * @brief Executes the next statement step.
   * @return SQLite result code.
   */
  auto Step [[nodiscard]] () const -> ResultCode;

  /**
   * @brief Gives the values of provided types from the current step
   * in the order they are refined in the statement.
   */
  auto GetStepValues [[nodiscard]] (
      const std::vector<sqldb::DataTypeVariant> &value_types) const
      -> std::vector<sqldb::Value>;

  /**
   * @brief Finalizes the statement.
   * @remark Other methods should not be called after this.
   */
  void Finalize();

 private:
  sqlite3_stmt *sqlite_statement_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_FACADE_H_
