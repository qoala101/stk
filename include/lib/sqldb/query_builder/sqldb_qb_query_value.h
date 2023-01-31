#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_

#include <string>

#include "cpp_meta_template_constructor.h"
#include "sqldb_concepts.h"
#include "sqldb_prm_types.h"
#include "sqldb_qb_condition.h"
#include "sqldb_qb_query_wrapper.h"
#include "sqldb_types.h"
#include "sqldb_value.h"

namespace vh::sqldb::qb {
class Select;

/**
 * @brief Converts inputs to parts of the query.
 */
class QueryValue : public QueryWrapper {
 public:
  template <ColumnDefinition Column>
  explicit QueryValue(cpp::meta::TemplateConstructor<Column> /*unused*/)
      : QueryValue{Column::GetFullName()} {}

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const Value &value);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const prm::QueryParam &param);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const Select &query_builder);

 private:
  explicit QueryValue(std::string column_name);
  explicit QueryValue(const prm::Parametrized<SelectQuery> &query);

  friend auto operator==(const QueryValue &left, const QueryValue &right)
      -> Condition;

  friend auto operator<(const QueryValue &left, const QueryValue &right)
      -> Condition;

  friend auto operator>=(const QueryValue &left, const QueryValue &right)
      -> Condition;
};
}  // namespace vh::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_QUERY_VALUE_H_