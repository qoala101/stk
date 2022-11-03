#include "sqldb_qb_query_value.h"

#include <fmt/format.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/concat.hpp>

#include "sqldb_p_types.h"
#include "sqldb_qb_query_wrapper.h"
#include "sqldb_qb_select.h"

namespace stonks::sqldb::qb {
namespace {
[[nodiscard]] auto ConditionFrom(std::string_view format,
                                 const QueryValue &left,
                                 const QueryValue &right) {
  const auto &left_query = left.GetQuery();
  Expects(!left_query.value.empty());

  const auto &right_query = right.GetQuery();
  Expects(!right_query.value.empty());

  auto query =
      fmt::format(fmt::runtime(format), left_query.value, right_query.value);
  auto params =
      ranges::views::concat(left_query.params.value, right_query.params.value) |
      ranges::to_vector;
  return Condition{{std::move(query), std::move(params)}};
}

[[nodiscard]] auto ToString(const Value &value) {
  return std::visit(
      [](const auto &v) -> std::string {
        using V = std::decay_t<decltype(v)>;

        if constexpr (std::is_same_v<V, std::string>) {
          return fmt::format(R"("{}")", v);
        }
        if constexpr (std::is_same_v<V, std::monostate>) {
          Expects(false);
        } else {
          return fmt::format("{}", v);
        }
      },
      value.value);
}
}  // namespace

QueryValue::QueryValue(const Value &value) : QueryWrapper{{ToString(value)}} {}

QueryValue::QueryValue(const p::QueryParam &param)
    : QueryWrapper{{"?", {{{param}}}}} {}

QueryValue::QueryValue(const Select &select) : QueryValue{select.Build()} {}

QueryValue::QueryValue(std::string column_name)
    : QueryWrapper{{std::move(column_name)}} {}

QueryValue::QueryValue(const p::Parametrized<SelectQuery> &query)
    : QueryWrapper{{fmt::format("({})", query.value), query.params}} {}

[[nodiscard]] auto operator==(const QueryValue &left, const QueryValue &right)
    -> Condition {
  return ConditionFrom("{} == {}", left, right);
}

[[nodiscard]] auto operator<(const QueryValue &left, const QueryValue &right)
    -> Condition {
  return ConditionFrom("{} < {}", left, right);
}

[[nodiscard]] auto operator>=(const QueryValue &left, const QueryValue &right)
    -> Condition {
  return ConditionFrom("{} >= {}", left, right);
}
}  // namespace stonks::sqldb::qb