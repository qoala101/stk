/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "sqldb_qb_select.h"

#include <absl/strings/str_join.h>
#include <fmt/core.h>

#include <gsl/assert>
#include <magic_enum.hpp>
#include <not_null.hpp>
#include <range/v3/detail/variant.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/utility/get.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_prm_types.h"
#include "sqldb_qb_types.h"
#include "sqldb_types.h"

template <>
constexpr auto magic_enum::customize::enum_name(
    vh::sqldb::qb::Order value) noexcept -> magic_enum::customize::customize_t {
  switch (value) {
    case vh::sqldb::qb::Order::kAscending:
      return "ASC";
    case vh::sqldb::qb::Order::kDescending:
      return "DESC";
    default:
      return invalid_tag;
  }
}

namespace vh::sqldb::qb {
namespace {
auto DistinctQueryFrom [[nodiscard]] (bool distinct) {
  return distinct ? "DISTINCT " : "";
}

auto ColumnsQueryFrom
    [[nodiscard]] (const std::vector<SelectColumnData>& select_columns_data) {
  Expects(!select_columns_data.empty());

  return absl::StrJoin(
      select_columns_data | ranges::views::transform([](const auto& column) {
        return column.full_name;
      }),
      ", ");
}

auto ResultDefinitionFrom
    [[nodiscard]] (const std::vector<SelectColumnData>& select_columns_data) {
  Expects(!select_columns_data.empty());

  return select_columns_data | ranges::views::transform([](const auto& column) {
           return ColumnType{.column = {column.name}, .type = column.type};
         }) |
         ranges::to_vector;
}
}  // namespace

Select::Select(All /*unused*/, bool distinct)
    : select_all_{true},
      distinct_query_{DistinctQueryFrom(distinct)},
      columns_query_{"*"} {
  Ensures(select_all_);
  Ensures(!columns_query_->empty());
}

Select::Select(One /*unused*/)
    : columns_query_{"1"},
      result_definition_{{{{.column = {"1"}, .type = {DataType<int>{}}}}}} {
  Ensures(!columns_query_->empty());
  Ensures(!result_definition_->empty());
}

auto Select::Where(WhereCondition condition) -> Select& {
  auto& where_query = condition.GetQuery();
  Expects(!where_query->empty());

  where_query_ = std::move(where_query);
  Ensures(!where_query_->empty());
  return *this;
}

auto Select::Limit(const QueryValue& value) -> Select& {
  Expects(limit_query_->empty());

  const auto& value_query = value.GetQuery();
  Expects(!value_query->empty());

  *limit_query_ = fmt::format(" LIMIT {}", *value_query);
  limit_query_.params = value_query.params;
  Ensures(!limit_query_->empty());
  return *this;
}

auto Select::Build() const -> prm::Parametrized<SelectQuery> {
  Expects(!table_name_->empty());
  Expects(!columns_query_->empty());

  auto query = fmt::format("SELECT {}{} FROM {}{}{}{}{}", *distinct_query_,
                           *columns_query_, *table_name_, *join_query_,
                           *where_query_, *order_by_query_, *limit_query_);
  auto params = ranges::views::concat(*join_query_.params, *where_query_.params,
                                      *limit_query_.params) |
                ranges::to_vector;

  return {{std::move(query), result_definition_}, std::move(params)};
}

Select::Select(const std::vector<SelectColumnData>& select_columns_data,
               bool distinct)
    : distinct_query_{DistinctQueryFrom(distinct)},
      columns_query_{ColumnsQueryFrom(select_columns_data)},
      result_definition_{ResultDefinitionFrom(select_columns_data)} {}

auto Select::From(std::string table_name,
                  cpp::Lazy<std::vector<SelectColumnData>> select_columns_data)
    -> Select& {
  Expects(table_name_->empty());
  Expects(!table_name.empty());
  *table_name_ = std::move(table_name);

  if (select_all_) {
    *result_definition_ = ResultDefinitionFrom(*select_columns_data);
  }

  Ensures(!table_name_->empty());
  return *this;
}

auto Select::Join(std::string_view table_name, const OnCondition& condition)
    -> Select& {
  const auto& condition_query = condition.GetQuery();
  Expects(!condition_query->empty());

  *join_query_ += fmt::format(" JOIN {}{}", table_name, *condition_query);
  join_query_.params += condition_query.params;

  Ensures(!join_query_->empty());
  return *this;
}

auto Select::OrderBy(std::string_view column_name, Order order) -> Select& {
  if (order_by_query_->empty()) {
    *order_by_query_ = " ORDER BY";
  } else {
    *order_by_query_ += ",";
  }

  *order_by_query_ +=
      fmt::format(" {} {}", column_name, magic_enum::enum_name(order));
  Ensures(!order_by_query_->empty());
  return *this;
}
}  // namespace vh::sqldb::qb