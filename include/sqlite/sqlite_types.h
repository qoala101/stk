#ifndef STONKS_SQLITE_SQLITE_TYPES_H_
#define STONKS_SQLITE_SQLITE_TYPES_H_

#include <sqlite3.h>

#include <memory>

#include "not_null.hpp"

/**
 * @file Common SQLite types.
 */
namespace stonks::sqlite {
struct SqliteDbCloser {
  void operator()(sqlite3* sqlite_db) noexcept;
};

struct SqliteStatementFinalizer {
  void operator()(sqlite3_stmt* sqlite_statement) noexcept;
};

/**
 * @brief Closes DB connection when destroyed.
 */
using SqliteDbHandle = cpp::not_null<std::unique_ptr<sqlite3, SqliteDbCloser>>;

/**
 * @brief Finalizes statement when destroyed.
 */
using SqliteStatementHandle =
    cpp::not_null<std::unique_ptr<sqlite3_stmt, SqliteStatementFinalizer>>;
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_TYPES_H_
