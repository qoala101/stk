#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_

#include "sqldb_p_types.h"
#include "sqldb_value.h"

namespace stonks::sqldb::qb {
class Select;

/**
 * @brief Converts objects to parts of the query.
 */
class QueryValue {
 public:
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const Value &value);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(p::Param param);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const Select &select);

  /**
   * @brief Gives the query build from inputs.
   */
  [[nodiscard]] auto GetQuery() const -> const p::Parametrized<Query> &;
  [[nodiscard]] auto GetQuery() -> p::Parametrized<Query> &;

 private:
  explicit QueryValue(const p::Parametrized<SelectQuery> &query);

  [[nodiscard]] static auto GetQueryImpl(cpp::This<QueryValue> auto &t)
      -> auto &;

  p::Parametrized<Query> query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_
