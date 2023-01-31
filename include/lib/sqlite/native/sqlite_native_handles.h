#ifndef VH_SQLITE_NATIVE_HANDLES_H_
#define VH_SQLITE_NATIVE_HANDLES_H_

#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "log_i_logger.h"

class sqlite3;
class sqlite3_stmt;

/**
 * @file RAII wrappers of SQLite objects.
 */

namespace vh::sqlite {
namespace detail {
class NativeDbCloser {
 public:
  /**
   * @remark Not for use. Added for not null to compile.
   */
  NativeDbCloser() = default;

  explicit NativeDbCloser(cpp::NnSp<log::ILogger> logger);

  void operator()(sqlite3 *db) noexcept;

 private:
  cpp::Sp<log::ILogger> logger_{};
};

class NativeStatementFinalizer {
 public:
  /**
   * @remark Not for use. Added for not null to compile.
   */
  NativeStatementFinalizer() = default;

  explicit NativeStatementFinalizer(cpp::NnSp<log::ILogger> logger);

  void operator()(sqlite3_stmt *statement) noexcept;

 private:
  cpp::Sp<log::ILogger> logger_{};
};
}  // namespace detail

/**
 * @remark Closes connection when destroyed.
 */
using NativeDbHandle = cpp::NnUp<sqlite3, detail::NativeDbCloser>;

/**
 * @remark Finalizes statement when destroyed.
 */
using NativeStatementHandle =
    cpp::NnUp<sqlite3_stmt, detail::NativeStatementFinalizer>;
}  // namespace vh::sqlite

#endif  // VH_SQLITE_NATIVE_HANDLES_H_
