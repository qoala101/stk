/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLITE_NATIVE_DB_FACADE_H_
#define VH_SQLITE_NATIVE_DB_FACADE_H_

#include <string_view>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "sqldb_types.h"
#include "sqlite_native_db_handles_factory.h"
#include "sqlite_native_handles.h"
#include "sqlite_types.h"

namespace vh::sqlite {
/**
 * @brief Convenience API for SQLite DB.
 */
class NativeDbFacade {
 public:
  explicit NativeDbFacade(cpp::NnSp<log::ILogger> logger);

  /**
   * @brief Replaces current data with the contents of the other DB.
   */
  static void CopyDataFrom(sqlite3 &source_db, sqlite3 &target_db);

  /**
   * @brief Enforces foreign keys on DB.
   * @remark Foreign keys are disabled by default.
   */
  static void EnableForeignKeys(sqlite3 &db);

  /**
   * @brief Sets native synchronization.
   * @remark Synchronization is enabled for new DBs by default.
   */
  static void SetSynchronizationEnabled(sqlite3 &db, bool enabled);

  /**
   * @brief Writes DB to file.
   */
  void WriteToFile(sqlite3 &db, const FilePath &file_path) const;

  /**
   * @brief Creates prepared statement for the query.
   */
  auto CreatePreparedStatement
      [[nodiscard]] (sqlite3 &db, const sqldb::Query &query) const
      -> NativeStatementHandle;

  /**
   * @brief Closes DB.
   * @remark Other methods should not be called after this.
   */
  void Close(sqlite3 &db) const;

 private:
  static void SetPragma(sqlite3 &db, std::string_view pragma,
                        std::string_view value);

  cpp::NnSp<log::ILogger> logger_;
  NativeDbHandlesFactory handles_factory_;
};
}  // namespace vh::sqlite

#endif  // VH_SQLITE_NATIVE_DB_FACADE_H_
