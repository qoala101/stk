#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_

#include <fmt/format.h>

#include <vector>

#include "cpp_this.h"
#include "cpp_typed_struct.h"
#include "cpp_views.h"
#include "sqldb_qb_condition.h"
#include "sqldb_table_traits.h"
#include "sqldb_types.h"
#include "sqldb_value.h"

/**
 * @file Symbols used by query builders.
 */

namespace stonks::sqldb::qb {
struct All;
struct One;

template <typename ColumnT>
struct Column {};

class Select;

struct Param {
  Param() : text_{"?"} {}

  // explicit Param(const Select &builder);

  std::string text_{};
};

/**
 * @brief Converts objects to parts of the query.
 */
class QueryValue {
 public:
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const Value &value) { query_ = value.ToString(); }
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  QueryValue(const Param &param) { query_ = param.text_; }

  QueryValue(const Select &builder);

  [[nodiscard]] auto GetQuery() const -> const std::string & { return query_; }
  [[nodiscard]] auto GetQuery() -> std::string & { return query_; }

 private:
  [[nodiscard]] static auto GetQueryImpl(cpp::This<QueryValue> auto &t)
      -> auto &;

  std::string query_{};
};

/**
 * @brief Builds WHERE-query from condition.
 */
class WhereCondition {
 public:
  /**
   * @param condition Where condition.
   */
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  WhereCondition(const Condition &condition);

  /**
   * @brief Gives the value of the query.
   */
  [[nodiscard]] auto GetQuery() const -> const std::string &;
  [[nodiscard]] auto GetQuery() -> std::string &;

 private:
  [[nodiscard]] static auto GetQueryImpl(cpp::This<WhereCondition> auto &t)
      -> auto &;

  std::string query_{};
};

[[nodiscard]] auto Exists(const Select &builder) -> Condition;

[[nodiscard]] inline auto On(const Condition &where) { return where; };

template <typename LeftColumn, typename RightColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              const Column<RightColumn> &right) {
  return Condition{fmt::format("{} = {}",
                               ColumnTraits<LeftColumn>::GetFullName(),
                               ColumnTraits<RightColumn>::GetFullName())};
}

template <typename LeftColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              const Param &right) {
  return Condition{fmt::format(
      "{} = {}", ColumnTraits<LeftColumn>::GetFullName(), right.text_)};
}

[[nodiscard]] auto operator==(std::string_view left, const Select &builder)
    -> Condition;

template <typename LeftColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              const Select &builder) {
  return ColumnTraits<LeftColumn>::GetFullName() == builder;
}

template <typename LeftColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              std::string_view right) {
  return Condition{fmt::format(R"({} = "{}")",
                               ColumnTraits<LeftColumn>::GetFullName(), right)};
}

template <typename LeftColumn>
[[nodiscard]] auto operator>=(const Column<LeftColumn> &left,
                              const Param &right) {
  return Condition{fmt::format(
      "{} >= {}", ColumnTraits<LeftColumn>::GetFullName(), right.text_)};
}

template <typename LeftColumn>
[[nodiscard]] auto operator<(const Column<LeftColumn> &left,
                             const Param &right) {
  return Condition{fmt::format(
      "{} < {}", ColumnTraits<LeftColumn>::GetFullName(), right.text_)};
}
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_
