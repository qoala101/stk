#ifndef STONKS_SQLITE_SQLITE_RAW_HANDLES_H_
#define STONKS_SQLITE_SQLITE_RAW_HANDLES_H_

#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "log_i_logger.h"

class sqlite3;
class sqlite3_stmt;

/**
 * @file RAII wrappers of SQLite objects.
 */

namespace stonks::sqlite {
namespace detail {
class SqliteDbCloser {
 public:
  /**
   * @remark Not for use. Added for not null to compile.
   */
  SqliteDbCloser() = default;

  explicit SqliteDbCloser(cpp::NnSp<log::ILogger> logger);

  void operator()(sqlite3 *sqlite_db) noexcept;

 private:
  cpp::Sp<log::ILogger> logger_;
};

class SqliteStatementFinalizer {
 public:
  /**
   * @remark Not for use. Added for not null to compile.
   */
  SqliteStatementFinalizer() = default;

  explicit SqliteStatementFinalizer(cpp::NnSp<log::ILogger> logger);

  void operator()(sqlite3_stmt *sqlite_statement) noexcept;

 private:
  cpp::Sp<log::ILogger> logger_;
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
