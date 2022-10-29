#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_WRAPPER_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_WRAPPER_H_

#include "sqldb_p_types.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
/**
 * @brief Wraps the query.
 */
class QueryWrapper {
 public:
  /**
   * @brief Gives the value of the query.
   */
  [[nodiscard]] auto GetQuery() const -> const p::Parametrized<Query> &;
  [[nodiscard]] auto GetQuery() -> p::Parametrized<Query> &;

 protected:
  explicit QueryWrapper(p::Parametrized<Query> query);

 private:
  [[nodiscard]] static auto GetQueryImpl(cpp::This<QueryWrapper> auto &t)
      -> auto &;

  p::Parametrized<Query> query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_WRAPPER_H_
