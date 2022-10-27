#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_WHERE_CONDITION_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_WHERE_CONDITION_H_

#include "sqldb_p_types.h"
#include "sqldb_qb_condition.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
/**
 * @brief Builds WHERE-query from condition.
 */
class WhereCondition {
 public:
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  WhereCondition(const Condition &condition);

  /**
   * @brief Gives the value of the query.
   */
  [[nodiscard]] auto GetQuery() const -> const p::Parametrized<Query> &;
  [[nodiscard]] auto GetQuery() -> p::Parametrized<Query> &;

 private:
  [[nodiscard]] static auto GetQueryImpl(cpp::This<WhereCondition> auto &t)
      -> auto &;

  p::Parametrized<Query> query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_WHERE_CONDITION_H_
