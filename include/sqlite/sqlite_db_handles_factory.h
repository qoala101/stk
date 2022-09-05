#ifndef STONKS_SQLITE_SQLITE_DB_HANDLES_FACTORY_H_
#define STONKS_SQLITE_SQLITE_DB_HANDLES_FACTORY_H_

#include <string_view>

#include "sqlite_raw_handles.h"

/**
 * @file Constructors of SQLite handles.
 */

namespace stonks::sqlite::db_handles_factory {
/**
 * @brief Creates in-memory SQLite DB.
 */
[[nodiscard]] auto CreateInMemoryDb() -> SqliteDbHandle;

/**
 * @brief Creates SQLite handle to DB from file.
 */
[[nodiscard]] auto CreateHandleToFileDb(std::string_view file_path)
    -> SqliteDbHandle;

/**
 * @brief Reads SQLite DB from file and copies the contents to the in-memory DB.
 * @return Handle to in-memory DB.
 */
[[nodiscard]] auto LoadDbFromFileToMemory(std::string_view file_path)
    -> SqliteDbHandle;
}  // namespace stonks::sqlite::db_handles_factory

#endif  // STONKS_SQLITE_SQLITE_DB_HANDLES_FACTORY_H_
