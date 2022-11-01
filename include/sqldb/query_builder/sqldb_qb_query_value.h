#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_

#include "sqldb_p_types.h"
#include "sqldb_qb_condition.h"
#include "sqldb_qb_query_wrapper.h"
#include "sqldb_table_traits.h"
#include "sqldb_value.h"

namespace stonks::sqldb::qb {
class Select;

/**
 * @brief Converts inputs to parts of the query.
 */
class QueryValue : public QueryWrapper {
 public:
  template <typename Column>
  explicit QueryValue(Column * /*unused*/)
      : QueryValue{ColumnTraits<Column>::GetFullName()} {}

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const Value &value);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const p::QueryParam &param);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const Select &select);

 private:
  explicit QueryValue(std::string column_name);
  explicit QueryValue(const p::Parametrized<SelectQuery> &query);

  friend auto operator==(const QueryValue &left, const QueryValue &right)
      -> Condition;

  friend auto operator<(const QueryValue &left, const QueryValue &right)
      -> Condition;

  friend auto operator>=(const QueryValue &left, const QueryValue &right)
      -> Condition;
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_
