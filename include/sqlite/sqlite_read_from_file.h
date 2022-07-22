#ifndef STONKS_SQLITE_SQLITE_UTILS_H_
#define STONKS_SQLITE_SQLITE_UTILS_H_

#include <string_view>

#include "sqlite_types.h"

/**
 * @file SQLite utils related to reading from file.
 */
namespace stonks::sqlite::read_from_file {
/**
 * @brief Reads SQLite DB from file.
 * @return Handle to file DB.
 */
[[nodiscard]] auto OpenSqliteDbFromFile(std::string_view file_path)
    -> SqliteDbHandle;

/**
 * @brief Reads SQLite DB from file and copies the contents to the in-memory DB.
 * @return In-memory DB.
 */
[[nodiscard]] auto LoadSqliteDbFromFileToMemory(std::string_view file_path)
    -> SqliteDbHandle;
}  // namespace stonks::sqlite::read_from_file

#endif  // STONKS_SQLITE_SQLITE_UTILS_H_
