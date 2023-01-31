#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CONDITION_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CONDITION_H_

#include <string_view>

#include "sqldb_prm_types.h"
#include "sqldb_qb_query_wrapper.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
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
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CONDITION_H_
