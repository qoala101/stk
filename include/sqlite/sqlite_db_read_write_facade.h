#ifndef STONKS_SQLITE_SQLITE_UTILS_H_
#define STONKS_SQLITE_SQLITE_UTILS_H_

#include <sqlite3.h>

#include <string_view>

#include "sqlite_types.h"

namespace stonks::sqlite {
/**
 * @brief Read-writing SQLite util.
 */
class SqliteDbReadWriteFacade {
 public:
  /**
   * @brief Read SQLite DB from file.
   */
  [[nodiscard]] static auto ReadSqliteDbFromFile(std::string_view file_path)
      -> SqliteDbHandle;

  /**
   * @brief Write SQLite DB to file.
   */
  static void WriteSqliteDbToFile(sqlite3 &sqlite_db,
                                  std::string_view file_path);
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_UTILS_H_
