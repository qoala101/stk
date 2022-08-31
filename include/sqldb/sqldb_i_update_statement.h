#ifndef STONKS_SQLDB_SQLDB_I_UPDATE_STATEMENT_H_
#define STONKS_SQLDB_SQLDB_I_UPDATE_STATEMENT_H_

#include <vector>

#include "sqldb_value.h"

namespace stonks::sqldb {
/**
 * @brief Statement which is expected to be prepared by DB once and later be
 * called many times.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IUpdateStatement {
 public:
  virtual ~IUpdateStatement() = default;

  /**
   * @brief Executes the statement.
   */
  void Execute() const;

  /**
   * @brief Executes the statement.
   * @param params Values to be injected into the query in the specified order.
   */
  virtual void Execute(std::vector<Value> params) const = 0;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_I_UPDATE_STATEMENT_H_
