#ifndef STONKS_SQLITE_SQLITE_PS_COMMON_IMPL_H_
#define STONKS_SQLITE_SQLITE_PS_COMMON_IMPL_H_

#include <vector>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqlite_native_db_handle_variant.h"
#include "sqlite_native_statement_facade.h"

namespace stonks::sqlite::ps {
/**
 * @brief Common API and fields for prepared statements implementation.
 * @remark Keeps DB alive while handle is alive.
 */
class CommonImpl {
 public:
  /**
   * @param prepared_statement_handle Handle for SQLite prepared statement
   * produced by parent DB.
   */
  CommonImpl(cpp::NnSp<NativeDbHandleVariant> native_db_handle,
             NativeStatementHandle native_statement_handle, sqldb::Query query,
             cpp::NnUp<log::ILogger> logger);

  /**
   * @brief Gives a facade to work with a statement.
   */
  auto GetNativeStatementFacade [[nodiscard]] () const
      -> const NativeStatementFacade &;

  /**
   * @brief Binds new params and logs the query before execution.
   */
  void BeforeExecution(const std::vector<sqldb::Value> &params) const;

 private:
  cpp::NnSp<NativeDbHandleVariant> native_db_handle_;
  NativeStatementHandle native_statement_handle_;
  sqldb::Query query_{};
  cpp::NnUp<log::ILogger> logger_;
  NativeStatementFacade native_statement_facade_;
};
}  // namespace stonks::sqlite::ps

#endif  // STONKS_SQLITE_SQLITE_PS_COMMON_IMPL_H_
