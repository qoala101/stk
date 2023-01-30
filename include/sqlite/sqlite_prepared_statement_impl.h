#ifndef STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_IMPL_H_
#define STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_IMPL_H_

#include <vector>

#include "cpp_meta_private_to.h"
#include "cpp_mutex.h"
#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqlite_native_db_handle_variant.h"
#include "sqlite_native_handles.h"
#include "sqlite_native_statement_facade.h"

namespace stonks::sqlite {
class Db;

/**
 * @brief Common API and fields for prepared statements implementation.
 * @remark Keeps DB alive while handle is alive.
 */
class PreparedStatementImpl {
 public:
  /**
   * @param native_statement_handle Handle for SQLite prepared statement
   * produced by parent DB.
   * @param mutex Protects lifetime and execution of native statement.
   */
  PreparedStatementImpl(cpp::meta::PrivateTo<Db>,
                        cpp::NnSp<NativeDbHandleVariant> native_db_handle,
                        NativeStatementHandle native_statement_handle,
                        sqldb::Query query, cpp::NnSp<log::ILogger> logger,
                        cpp::MutexVariant native_statement_mutex);

  PreparedStatementImpl(const PreparedStatementImpl &) = delete;
  PreparedStatementImpl(PreparedStatementImpl &&) noexcept = default;

  auto operator=(const PreparedStatementImpl &)
      -> PreparedStatementImpl & = delete;
  auto operator=(PreparedStatementImpl &&) noexcept
      -> PreparedStatementImpl & = default;

  ~PreparedStatementImpl();

  /**
   * @brief Gives the statement.
   */
  auto GetNativeStatement [[nodiscard]] () const -> sqlite3_stmt &;

  /**
   * @brief Binds new params and logs the query before execution.
   * @remark Lock should be kept alive during execution.
   */
  auto PrepareExecutionAndLock
      [[nodiscard]] (const std::vector<sqldb::Value> &params) const
      -> cpp::LockVariant;

 private:
  cpp::NnSp<NativeDbHandleVariant> native_db_handle_;
  cpp::Nullable<NativeStatementHandle> native_statement_handle_;
  sqldb::Query query_{};
  cpp::NnSp<log::ILogger> logger_;
  cpp::MutexVariant native_statement_mutex_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_PREPARED_STATEMENT_IMPL_H_
