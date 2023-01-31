#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_WRAPPED_CONDITIONS_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_WRAPPED_CONDITIONS_H_

#include <string_view>

#include "sqldb_qb_condition.h"
#include "sqldb_qb_query_wrapper.h"

namespace stonks::sqldb::qb {
/**
 * @brief Wraps condition query.
 */
class WrappedCondition : public QueryWrapper {
 protected:
  WrappedCondition(const Condition &condition, std::string_view format);
};

/**
 * @brief Builds ON-query from condition.
 */
class OnCondition : public WrappedCondition {
 public:
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  OnCondition(const Condition &condition);
};

/**
 * @brief Builds WHERE-query from condition.
 */
class WhereCondition : public WrappedCondition {
 public:
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  WhereCondition(const Condition &condition);
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_WRAPPED_CONDITIONS_H_
