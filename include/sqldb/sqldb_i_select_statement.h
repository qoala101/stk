#ifndef STONKS_SQLDB_SQLDB_I_SELECT_STATEMENT_H_
#define STONKS_SQLDB_SQLDB_I_SELECT_STATEMENT_H_

#include <vector>

#include "sqldb_rows.h"
#include "sqldb_value.h"

namespace stonks::sqldb {
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
  [[nodiscard]] auto Execute() const -> Rows;

  /**
   * @brief Executes the statement and gives result.
   * @param params Values to be injected into the query in the specified order.
   */
  [[nodiscard]] virtual auto Execute(const std::vector<Value> &params) const
      -> Rows = 0;

 protected:
  explicit ISelectStatement() = default;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_I_SELECT_STATEMENT_H_
