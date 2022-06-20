#ifndef STONKS_SQLITE_SQLITE_UTILS_H_
#define STONKS_SQLITE_SQLITE_UTILS_H_

#include <sqlite3.h>

#include <gsl/pointers>
#include <string_view>

#include "sqlite_types.h"

namespace stonks::sqlite {
/**
 * @brief Read SQLite DB from file.
 */
[[nodiscard]] auto ReadSqliteDbFromFile(std::string_view file_path)
    -> SqliteDbHandle;

/**
 * @brief Write SQLite DB to file.
 */
void WriteSqliteDbToFile(sqlite3 &sqlite_db, std::string_view file_path);
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_UTILS_H_
