#ifndef STONKS_SQLITE_SQLITE_RAW_HANDLES_H_
#define STONKS_SQLITE_SQLITE_RAW_HANDLES_H_

#include "cpp_not_null.h"

class sqlite3;
class sqlite3_stmt;

/**
 * @file RAII wrappers of SQLite objects.
 */

namespace stonks::sqlite {
namespace detail {
struct SqliteDbCloser {
  void operator()(sqlite3 *sqlite_db) noexcept;
};

struct SqliteStatementFinalizer {
  void operator()(sqlite3_stmt *sqlite_statement) noexcept;
};
}  // namespace detail

/**
 * @remark Closes connection when destroyed.
 */
using SqliteDbHandle = cpp::NnUp<sqlite3, detail::SqliteDbCloser>;

/**
 * @remark Finalizes statement when destroyed.
 */
using SqliteStatementHandle =
    cpp::NnUp<sqlite3_stmt, detail::SqliteStatementFinalizer>;
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_RAW_HANDLES_H_
