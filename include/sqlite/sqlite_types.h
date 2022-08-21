#ifndef STONKS_SQLITE_SQLITE_TYPES_H_
#define STONKS_SQLITE_SQLITE_TYPES_H_

#include <sqlite3.h>

#include "ccutils_not_null.h"

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
using SqliteDbHandle = ccutils::NnUp<sqlite3, SqliteDbCloser>;

/**
 * @brief Finalizes statement when destroyed.
 */
using SqliteStatementHandle =
    ccutils::NnUp<sqlite3_stmt, SqliteStatementFinalizer>;
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_TYPES_H_
