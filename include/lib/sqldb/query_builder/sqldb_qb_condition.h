/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLDB_QB_CONDITION_H_
#define VH_SQLDB_QB_CONDITION_H_

#include <string_view>

#include "sqldb_prm_types.h"
#include "sqldb_qb_query_wrapper.h"
#include "sqldb_types.h"

namespace vh::sqldb::qb {
/**
 * @brief API to build the condition.
 */
class Condition : public QueryWrapper {
 public:
  /**
   * @param query Initial condition query.
   */
  explicit Condition(prm::Parametrized<Query> query);

  /**
   * @brief Adds AND-condition to the query.
   */
  auto operator&& [[nodiscard]] (const Condition &other) -> Condition &;

  /**
   * @brief Adds OR-condition to the query.
   */
  auto operator|| [[nodiscard]] (const Condition &other) -> Condition &;

 private:
  void AppendCondition(const Condition &condition,
                       std::string_view operator_string);
};
}  // namespace vh::sqldb::qb

#endif  // VH_SQLDB_QB_CONDITION_H_
