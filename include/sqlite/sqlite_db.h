#ifndef STONKS_SQLITE_SQLITE_DB_H_
#define STONKS_SQLITE_SQLITE_DB_H_

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_types.h"
#include "sqlite_native_db_facade.h"
#include "sqlite_native_db_handle_variant.h"

namespace stonks::sqlite {
/**
 * @copydoc sqldb::IDb
 */
class Db : public sqldb::IDb {
 public:
  /**
   * @brief Creates wrapper for SQLite DB.
   */
  Db(cpp::NnUp<log::ILogger> logger, NativeDbHandleVariant native_db_handle);

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
  auto PreparedStatementImplFrom [[nodiscard]] (sqldb::Query query) const;

  cpp::NnSp<log::ILogger> logger_;
  cpp::NnSp<NativeDbHandleVariant> native_db_handle_;
  NativeDbFacade native_db_facade_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_H_
