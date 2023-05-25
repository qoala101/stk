/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLITE_DB_H_
#define VH_SQLITE_DB_H_

#include "cpp_factory.h"
#include "cpp_mutex.h"
#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_types.h"
#include "sqlite_native_db_facade.h"
#include "sqlite_native_db_handle_variant.h"
#include "sqlite_prepared_statement_impl.h"

namespace vh::sqlite {
/**
 * @copydoc sqldb::IDb
 */
class Db : public sqldb::IDb {
 public:
  /**
   * @brief Creates wrapper for SQLite DB.
   * @param prepared_statement_mutex_factory Produces mutexes for prepared
   * statements which determines their thread safety.
   */
  Db(cpp::NnUp<log::ILogger> logger, NativeDbHandleVariant native_db_handle,
     cpp::Factory<cpp::MutexVariant> prepared_statement_mutex_factory);

  /**
   * @copydoc sqldb::IDb::PrepareStatement
   */
  auto PrepareStatement [[nodiscard]] (sqldb::SelectQuery query)
  -> cpp::NnUp<sqldb::ISelectStatement> override;

  /**
   * @copydoc sqldb::IDb::PrepareStatement
   */
  auto PrepareStatement [[nodiscard]] (sqldb::Query query)
  -> cpp::NnUp<sqldb::IUpdateStatement> override;

 private:
  auto PreparedStatementImplFrom [[nodiscard]] (sqldb::Query query);

  cpp::NnSp<log::ILogger> logger_;
  cpp::NnSp<NativeDbHandleVariant> native_db_handle_;
  cpp::Factory<cpp::MutexVariant> prepared_statement_mutex_factory_;
  NativeDbFacade native_db_facade_;
};
}  // namespace vh::sqlite

#endif  // VH_SQLITE_DB_H_
