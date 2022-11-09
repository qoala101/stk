#ifndef STONKS_SQLITE_SQLITE_DB_H_
#define STONKS_SQLITE_SQLITE_DB_H_

#include "cpp_not_null.h"
#include "di_factory.h"
#include "log_i_logger.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_types.h"
#include "sqlite_db_facade.h"
#include "sqlite_db_handle_variant.h"

namespace stonks::sqlite {
/**
 * @copydoc sqldb::IDb
 */
class Db : public sqldb::IDb {
 public:
  /**
   * @brief Creates wrapper for SQLite DB.
   */
  Db(di::Factory<log::ILogger> logger_factory,
     SqliteDbHandleVariant sqlite_db_handle);

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
  auto PsCommonImplFrom [[nodiscard]] (sqldb::Query query) const;

  di::Factory<log::ILogger> logger_factory_;
  cpp::NnSp<SqliteDbHandleVariant> sqlite_db_handle_;
  DbFacade sqlite_db_facade_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_H_
