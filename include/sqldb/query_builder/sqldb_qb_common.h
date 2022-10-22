#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_

#include <vector>

#include "cpp_views.h"
#include "sqldb_types.h"
#include "sqldb_qb_select.h"
#include <fmt/format.h>

/**
 * @file Symbols used by query builders.
 */

namespace stonks::sqldb::qb {
template <typename ColumnT>
struct Column {};

struct Param {
  Param() : text_{"?"} {}

  explicit Param(const Select& builder)
      : text_{fmt::format("({})", builder.Build().query.value)} {}

  std::string text_{};
};

[[nodiscard]] inline auto Exists(Select builder) {
  return fmt::format("EXISTS ({})", builder.Build().query.value);
};

[[nodiscard]] inline auto On(std::string_view on_clause) {
  return fmt::format("ON ({})", on_clause);
};

template <typename LeftColumn, typename RightColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              const Column<RightColumn> &right) {
  return fmt::format("{} = {}", ColumnTraits<LeftColumn>::GetFullName(),
                     ColumnTraits<RightColumn>::GetFullName());
}

template <typename LeftColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              const Param &right) {
  return fmt::format("{} = {}", ColumnTraits<LeftColumn>::GetFullName(),
                     right.text_);
}

template <typename LeftColumn>
[[nodiscard]] auto operator==(const Column<LeftColumn> &left,
                              std::string_view right) {
  return fmt::format(R"({} = "{}")", ColumnTraits<LeftColumn>::GetFullName(),
                     right);
}

template <typename LeftColumn>
[[nodiscard]] auto operator>=(const Column<LeftColumn> &left,
                              const Param &right) {
  return fmt::format("{} >= {}", ColumnTraits<LeftColumn>::GetFullName(),
                     right.text_);
}

template <typename LeftColumn>
[[nodiscard]] auto operator<(const Column<LeftColumn> &left,
                             const Param &right) {
  return fmt::format("{} < {}", ColumnTraits<LeftColumn>::GetFullName(),
                     right.text_);
}
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_COMMON_H_
