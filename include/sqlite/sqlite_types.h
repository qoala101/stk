#ifndef STONKS_SQLITE_SQLITE_TYPES_H_
#define STONKS_SQLITE_SQLITE_TYPES_H_

#include <sqlite3.h>

#include <functional>
#include <memory>

namespace stonks::sqlite {
/**
 * @brief Closes DB connection when destroyed.
 */
using SqliteDbHandle = std::unique_ptr<sqlite3, std::function<void(sqlite3 *)>>;
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_TYPES_H_
