/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLITE_NATIVE_STATEMENT_FACADE_H_
#define VH_SQLITE_NATIVE_STATEMENT_FACADE_H_

#include <vector>

#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqlite_types.h"

class sqlite3_stmt;

namespace vh::sqlite {
/**
 * @brief Convenience API for SQLite prepared statement.
 */
class NativeStatementFacade {
 public:
  /**
   * @brief Resets prepared statement.
   * @remark Should be called before new parameters binding.
   */
  static void Reset(sqlite3_stmt &statement);

  /**
   * @brief Binds parameters in the order they defined in the statement.
   */
  static void BindParams(sqlite3_stmt &statement,
                         const std::vector<sqldb::Value> &params);

  /**
   * @brief Executes the next statement step.
   * @return SQLite result code.
   */
  static auto Step [[nodiscard]] (sqlite3_stmt &statement) -> ResultCode;

  /**
   * @brief Gives the values of provided types from the current step
   * in the order they are refined in the statement.
   */
  static auto GetStepValues
      [[nodiscard]] (sqlite3_stmt &statement,
                     const std::vector<sqldb::DataTypeVariant> &value_types)
      -> std::vector<sqldb::Value>;

  /**
   * @brief Finalizes the statement.
   * @remark Other methods should not be called after this.
   */
  static void Finalize(sqlite3_stmt &statement);
};
}  // namespace vh::sqlite

#endif  // VH_SQLITE_NATIVE_STATEMENT_FACADE_H_
