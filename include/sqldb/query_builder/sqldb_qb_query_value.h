#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_

#include "sqldb_p_types.h"
#include "sqldb_qb_condition.h"
#include "sqldb_qb_query_wrapper.h"
#include "sqldb_value.h"

namespace stonks::sqldb::qb {
class Select;

/**
 * @brief Converts objects to parts of the query.
 */
class QueryValue : public QueryWrapper {
 public:
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(std::string query);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(p::Parametrized<Query> query);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const Value &value);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(p::Param param);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const Select &select);

 private:
  friend auto operator==(const QueryValue &left, const QueryValue &right)
      -> Condition;

  friend auto operator<(const QueryValue &left, const QueryValue &right)
      -> Condition;

  friend auto operator>=(const QueryValue &left, const QueryValue &right)
      -> Condition;

  explicit QueryValue(const p::Parametrized<SelectQuery> &query);
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_
