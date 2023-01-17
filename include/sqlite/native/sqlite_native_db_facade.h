#ifndef STONKS_SQLITE_NATIVE_SQLITE_NATIVE_DB_FACADE_H_
#define STONKS_SQLITE_NATIVE_SQLITE_NATIVE_DB_FACADE_H_

#include "cpp_not_null.h"
#include "di_factory.h"
#include "log_i_logger.h"
#include "sqldb_types.h"
#include "sqlite_native_db_handles_factory.h"
#include "sqlite_native_handles.h"
#include "sqlite_types.h"

namespace stonks::sqlite {
/**
 * @brief Convenience API for SQLite DB.
 */
class NativeDbFacade {
 public:
  NativeDbFacade(di::Factory<log::ILogger> logger_factory,
           cpp::Nn<sqlite3 *> native_db);

  /**
   * @brief Writes DB to file.
   */
  void WriteToFile(const FilePath &file_path) const;

  /**
   * @brief Replaces current data with the contents of the other DB.
   */
  void CopyDataFrom(sqlite3 &other_db) const;

  /**
   * @brief Creates prepared statement for the query.
   */
  auto CreatePreparedStatement [[nodiscard]] (const sqldb::Query &query) const
      -> NativeStatementHandle;

  /**
   * @brief Enforces foreign keys on DB.
   * @remark Foreign keys are disabled by default.
   */
  void EnableForeignKeys() const;

  /**
   * @brief Turns native synchronization off.
   * @remark Synchronization is enabled by default.
   */
  void TurnOffSynchronization() const;

  /**
   * @brief Closes DB.
   * @remark Other methods should not be called after this.
   */
  void Close();

 private:
  void SetPragma(std::string_view pragma_name, std::string_view value) const;

  di::Factory<log::ILogger> logger_factory_;
  cpp::NnUp<log::ILogger> logger_;
  sqlite3 *native_db_{};
  NativeDbHandlesFactory handles_factory_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_NATIVE_SQLITE_NATIVE_DB_FACADE_H_
