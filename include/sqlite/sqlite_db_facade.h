#ifndef STONKS_SQLITE_SQLITE_DB_FACADE_H_
#define STONKS_SQLITE_SQLITE_DB_FACADE_H_

#include <sqlite3.h>

#include <string>
#include <string_view>

#include "not_null.hpp"
#include "sqlite_types.h"

namespace stonks::sqlite {
/**
 * @brief Convenience API for SQLite DB.
 */
class DbFacade {
 public:
  explicit DbFacade(cpp::not_null<sqlite3 *> sqlite_db);

  /**
   * @brief Gets the file from which DB was read.
   */
  [[nodiscard]] auto GetFileName() const -> std::string;

  /**
   * @brief Writes DB to file.
   */
  void WriteToFile(std::string_view file_path) const;

  /**
   * @brief Replaces current data with the contents of the other DB.
   */
  void CopyDataFrom(sqlite3 &other_db);

  /**
   * @brief Creates prepared statement for the query.
   */
  [[nodiscard]] auto CreatePreparedStatement(std::string_view query)
      -> SqliteStatementHandle;

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
