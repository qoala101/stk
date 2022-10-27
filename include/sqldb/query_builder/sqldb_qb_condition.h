#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CONDITION_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CONDITION_H_

#include <string_view>

#include "sqldb_p_types.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
/**
 * @brief API to build the condition.
 */
class Condition {
 public:
  /**
   * @param query Initial condition query.
   */
  explicit Condition(p::Parametrized<Query> query);

  /**
   * @brief Adds AND-condition to the query.
   */
  [[nodiscard]] auto operator&&(const Condition &condition) -> Condition &;

  /**
   * @brief Adds OR-condition to the query.
   */
  [[nodiscard]] auto operator||(const Condition &condition) -> Condition &;

  /**
   * @brief Gives the value of the query.
   */
  [[nodiscard]] auto GetQuery() const -> const p::Parametrized<Query> &;
  [[nodiscard]] auto GetQuery() -> p::Parametrized<Query> &;

 private:
  [[nodiscard]] static auto GetQueryImpl(cpp::This<Condition> auto &t)
      -> auto &;

  void AppendCondition(const Condition &condition,
                       std::string_view operator_string);

  p::Parametrized<Query> query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CONDITION_H_
