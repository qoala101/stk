#include "sqldb_qb_query_value.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <not_null.hpp>
#include <range/v3/detail/variant.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/utility/get.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/concat.hpp>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "cpp_typed_struct.h"
#include "sqldb_prm_types.h"
#include "sqldb_qb_query_wrapper.h"
#include "sqldb_qb_select.h"

namespace vh::sqldb::qb {
namespace {
auto ConditionFrom
    [[nodiscard]] (std::string_view format, const QueryValue &left,
                   const QueryValue &right) {
  const auto &left_query = left.GetQuery();
  Expects(!left_query->empty());

  const auto &right_query = right.GetQuery();
  Expects(!right_query->empty());

  auto query = fmt::format(fmt::runtime(format), *left_query, *right_query);
  auto params = ranges::views::concat(*left_query.params, *right_query.params) |
                ranges::to_vector;
  return Condition{{std::move(query), std::move(params)}};
}

auto ToString [[nodiscard]] (const Value &value) {
  return std::visit(
      [](const auto &v) -> std::string {
        using V = std::remove_cvref_t<decltype(v)>;

        if constexpr (std::is_same_v<V, std::string>) {
          return fmt::format(R"("{}")", v);
        }
        if constexpr (std::is_same_v<V, std::monostate>) {
          Expects(false);
        } else {
          return fmt::format("{}", v);
        }
      },
      *value);
}
}  // namespace

QueryValue::QueryValue(const Value &value) : QueryWrapper{{ToString(value)}} {}

QueryValue::QueryValue(const prm::QueryParam &param)
    : QueryWrapper{{"?", {{{param}}}}} {}

QueryValue::QueryValue(const Select &query_builder)
    : QueryValue{query_builder.Build()} {}

QueryValue::QueryValue(std::string column_name)
    : QueryWrapper{{std::move(column_name)}} {}

QueryValue::QueryValue(const prm::Parametrized<SelectQuery> &query)
    : QueryWrapper{{fmt::format("({})", *query), query.params}} {}

auto operator==(const QueryValue &left, const QueryValue &right) -> Condition {
  return ConditionFrom("{} == {}", left, right);
}

auto operator<(const QueryValue &left, const QueryValue &right) -> Condition {
  return ConditionFrom("{} < {}", left, right);
}

auto operator>=(const QueryValue &left, const QueryValue &right) -> Condition {
  return ConditionFrom("{} >= {}", left, right);
}
}  // namespace vh::sqldb::qb