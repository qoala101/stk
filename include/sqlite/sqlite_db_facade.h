#ifndef STONKS_SQLITE_SQLITE_DB_FACADE_H_
#define STONKS_SQLITE_SQLITE_DB_FACADE_H_

#include <string_view>

#include "cpp_not_null.h"
#include "sqlite_raw_handles.h"

namespace stonks::sqlite {
/**
 * @brief Convenience API for SQLite DB.
 */
class DbFacade {
 public:
  explicit DbFacade(cpp::Nn<sqlite3 *> sqlite_db);

  /**
   * @brief Writes DB to file.
   */
  void WriteToFile(std::string_view file_path) const;

  /**
   * @brief Replaces current data with the contents of the other DB.
   */
  void CopyDataFrom(sqlite3 &other_db) const;

  /**
   * @brief Creates prepared statement for the query.
   */
  [[nodiscard]] auto CreatePreparedStatement(std::string_view query) const
      -> SqliteStatementHandle;

  /**
   * @brief Enforces foreign keys on DB.
   * @remark Foreign keys are disabled by default.
   */
  void EnableForeignKeys() const;

  /**
   * @brief Closes DB.
   * @remark Other methods should not be called after this.
   */
  void Close();

 private:
  sqlite3 *sqlite_db_{};
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_FACADE_H_
