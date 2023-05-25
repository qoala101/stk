/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLDB_I_SELECT_STATEMENT_H_
#define VH_SQLDB_I_SELECT_STATEMENT_H_

#include <vector>

#include "sqldb_rows.h"
#include "sqldb_value.h"

namespace vh::sqldb {
/**
 * @brief Select statement which is expected to be prepared by DB once and later
 * be called many times.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class ISelectStatement {
 public:
  virtual ~ISelectStatement() = default;

  /**
   * @brief Executes the statement and gives result.
   */
  auto Execute [[nodiscard]] () const -> Rows;

  /**
   * @brief Executes the statement and gives result.
   * @param params Values to be injected into the query in the specified order.
   */
  virtual auto Execute [[nodiscard]] (std::vector<Value> params) const
      -> Rows = 0;
};
}  // namespace vh::sqldb

#endif  // VH_SQLDB_I_SELECT_STATEMENT_H_
