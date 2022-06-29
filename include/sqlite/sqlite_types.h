#ifndef STONKS_SQLITE_SQLITE_TYPES_H_
#define STONKS_SQLITE_SQLITE_TYPES_H_

#include <sqlite3.h>

#include <gsl/pointers>
#include <memory>

/**
 * @file Common SQLite types.
 */
namespace stonks::sqlite {
struct SqliteDbCloser {
  void operator()(gsl::not_null<sqlite3*> sqlite_db) noexcept;
};

struct SqliteStatementFinalizer {
  void operator()(gsl::not_null<sqlite3_stmt*> sqlite_statement) noexcept;
};

/**
 * @brief Closes DB connection when destroyed.
 */
using SqliteDbHandle = std::unique_ptr<sqlite3, SqliteDbCloser>;

/**
 * @brief Finalizes statement when destroyed.
 */
using SqliteStatementHandle =
    std::unique_ptr<sqlite3_stmt, SqliteStatementFinalizer>;
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_TYPES_H_
