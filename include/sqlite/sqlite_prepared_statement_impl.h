#ifndef STONKS_SQLITE_SQLITE_PS_COMMON_IMPL_H_
#define STONKS_SQLITE_SQLITE_PS_COMMON_IMPL_H_

#include <vector>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqlite_native_db_handle_variant.h"
#include "sqlite_native_handles.h"
#include "sqlite_native_statement_facade.h"

namespace stonks::sqlite {
/**
 * @brief Common API and fields for prepared statements implementation.
 * @remark Keeps DB alive while handle is alive.
 */
class PreparedStatementImpl {
 public:
  /**
   * @param prepared_statement_handle Handle for SQLite prepared statement
   * produced by parent DB.
   */
  PreparedStatementImpl(cpp::NnSp<NativeDbHandleVariant> native_db_handle,
                        NativeStatementHandle native_statement_handle,
                        sqldb::Query query, cpp::NnSp<log::ILogger> logger);

  /**
   * @brief Gives the statement.
   */
  auto GetNativeStatement [[nodiscard]] () const -> sqlite3_stmt &;

  /**
   * @brief Binds new params and logs the query before execution.
   */
  void BeforeExecution(const std::vector<sqldb::Value> &params) const;

 private:
  cpp::NnSp<NativeDbHandleVariant> native_db_handle_;
  NativeStatementHandle native_statement_handle_;
  sqldb::Query query_{};
  cpp::NnSp<log::ILogger> logger_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_PS_COMMON_IMPL_H_
