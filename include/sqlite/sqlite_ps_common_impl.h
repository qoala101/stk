#ifndef STONKS_SQLITE_SQLITE_PS_COMMON_IMPL_H_
#define STONKS_SQLITE_SQLITE_PS_COMMON_IMPL_H_

#include <vector>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqlite_prepared_statement_facade.h"
#include "sqlite_prepared_statement_handle.h"

namespace stonks::sqlite::ps {
/**
 * @brief Common API and fields for prepared statements implementation.
 */
class CommonImpl {
 public:
  /**
   * @param prepared_statement_handle Handle for SQLite prepared statement
   * produced by parent DB.
   */
  CommonImpl(PreparedStatementHandle prepared_statement_handle,
             sqldb::Query query, cpp::NnUp<log::ILogger> logger);

  /**
   * @brief Gives a facade to work with a statement.
   */
  [[nodiscard]] auto GetPreparedStatementFacade() const
      -> const PreparedStatementFacade &;

  /**
   * @brief Binds new params and logs the query before execution.
   */
  void BeforeExecution(const std::vector<sqldb::Value> &params) const;

 private:
  PreparedStatementHandle prepared_statement_handle_;
  sqldb::Query query_{};
  cpp::NnUp<log::ILogger> logger_;
  PreparedStatementFacade prepared_statement_facade_;
};
}  // namespace stonks::sqlite::ps

#endif  // STONKS_SQLITE_SQLITE_PS_COMMON_IMPL_H_
