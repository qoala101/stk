#ifndef STONKS_DB_DB_PREPARED_STATEMENT_H_
#define STONKS_DB_DB_PREPARED_STATEMENT_H_

#include <vector>

#include "db_row.h"
#include "db_row_definition.h"
#include "db_value.h"

namespace stonks::db {
/**
 * @brief Statement which is expected to be prepared by DB once and later be
 * called many times.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class PreparedStatement {
 public:
  virtual ~PreparedStatement() = default;

  /**
   * @brief Executes statement which doesn't return result.
   */
  void Execute();

  /**
   * @brief Executes statement which doesn't return result.
   * @param params Values to be injected into the query.
   */
  void Execute(const std::vector<Value> &params);

  /**
   * @brief Executes statement which returns rows.
   * @param result_definition Description of expected result rows.
   */
  auto Execute(const RowDefinition &result_definition) -> Rows;

  /**
   * @brief Executes statement which returns rows.
   * @param params Values to be injected into the query.
   * @param result_definition Description of expected result rows.
   */
  [[nodiscard]] virtual auto Execute(const std::vector<Value> &params,
                                     const RowDefinition &result_definition)
      -> Rows = 0;
};
}  // namespace stonks::db

#endif  // STONKS_DB_DB_PREPARED_STATEMENT_H_
