#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_

#include <fmt/format.h>

#include <vector>

#include "cpp_typed_struct.h"
#include "cpp_views.h"
#include "sqldb_table_traits.h"
#include "sqldb_types.h"

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

  explicit Param(const Select &builder);

  std::string text_{};
};

struct WhereQuery : public cpp::TypedStruct<std::string> {};

[[nodiscard]] auto Exists(const Select &builder) -> WhereQuery;

[[nodiscard]] inline auto On(const WhereQuery &where) {
  return fmt::format("ON ({})", where.value);
};

template <typename LeftColumn, typename RightColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              const Column<RightColumn> &right) {
  return WhereQuery{fmt::format("{} = {}",
                                ColumnTraits<LeftColumn>::GetFullName(),
                                ColumnTraits<RightColumn>::GetFullName())};
}

template <typename LeftColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              const Param &right) {
  return WhereQuery{fmt::format(
      "{} = {}", ColumnTraits<LeftColumn>::GetFullName(), right.text_)};
}

template <typename LeftColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              std::string_view right) {
  return WhereQuery{fmt::format(
      R"({} = "{}")", ColumnTraits<LeftColumn>::GetFullName(), right)};
}

template <typename LeftColumn>
[[nodiscard]] auto operator>=(const Column<LeftColumn> &left,
                              const Param &right) {
  return WhereQuery{fmt::format(
      "{} >= {}", ColumnTraits<LeftColumn>::GetFullName(), right.text_)};
}

template <typename LeftColumn>
[[nodiscard]] auto operator<(const Column<LeftColumn> &left,
                             const Param &right) {
  return WhereQuery{fmt::format(
      "{} < {}", ColumnTraits<LeftColumn>::GetFullName(), right.text_)};
}
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_
