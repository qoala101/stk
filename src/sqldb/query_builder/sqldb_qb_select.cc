#include "sqldb_qb_select.h"

#include <absl/strings/str_join.h>
#include <fmt/core.h>

#include <gsl/assert>
#include <iostream>
#include <memory>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_p_types.h"
#include "sqldb_qb_common.h"
#include "sqldb_qb_types.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
Select::Select(All* /*unused*/) : select_all_{true}, columns_query_{"*"} {
  Ensures(select_all_);
  Ensures(!columns_query_.value.empty());
}

Select::Select(One* /*unused*/)
    : columns_query_{"1"},
      result_definition_{{{{.column = {"1"}, .type = {DataType<int>{}}}}}} {
  Ensures(!columns_query_.value.empty());
  Ensures(!result_definition_.value.empty());
}

auto Select::Where(WhereCondition condition) -> Select& {
  auto& where_query = condition.GetQuery();
  Expects(!where_query.value.empty());
  where_query_ = std::move(where_query);
  Ensures(!where_query_.value.empty());
  return *this;
}

auto Select::Limit(const QueryValue& value) -> Select& {
  const auto& value_query = value.GetQuery();
  Expects(!value_query.value.empty());
  limit_query_.value = fmt::format(" LIMIT {}", value_query.value);
  limit_query_.params = value_query.params;
  Ensures(!limit_query_.value.empty());
  return *this;
}

auto Select::Build() const -> p::Parametrized<SelectQuery> {
  Expects(!table_name_.value.empty());
  Expects(!columns_query_.value.empty());

  auto query = fmt::format("SELECT {} FROM {}{}{}{}", columns_query_.value,
                           table_name_.value, join_query_.value,
                           where_query_.value, limit_query_.value);
  auto params =
      ranges::views::concat(join_query_.params.value, where_query_.params.value,
                            limit_query_.params.value) |
      ranges::to_vector;

  return {{std::move(query), result_definition_}, std::move(params)};
}

Select::Select(const std::vector<SelectColumnData>& select_columns_data) {
  SetColumnsQueryFrom(select_columns_data);
  SetResultDefinitionFrom(select_columns_data);
}

auto Select::From(
    std::string table_name,
    const cpp::Lazy<std::vector<SelectColumnData>>& select_columns_data)
    -> Select& {
  Expects(table_name_.value.empty());
  Expects(!table_name.empty());
  table_name_.value = std::move(table_name);

  if (select_all_) {
    SetResultDefinitionFrom(*select_columns_data);
  }

  Ensures(!table_name_.value.empty());
  return *this;
}

auto Select::Join(std::string_view table_name, const OnCondition& condition)
    -> Select& {
  const auto& condition_query = condition.GetQuery();
  Expects(!condition_query.value.empty());

  join_query_.value +=
      fmt::format(" JOIN {}{}", table_name, condition_query.value);
  join_query_.params.Append(condition_query.params);

  Ensures(!join_query_.value.empty());
  return *this;
}

void Select::SetColumnsQueryFrom(
    const std::vector<SelectColumnData>& select_columns_data) {
  Expects(!select_columns_data.empty());

  columns_query_.value = absl::StrJoin(
      select_columns_data |
          ranges::views::transform(
              [](const SelectColumnData& column) { return column.full_name; }),
      ", ");

  Ensures(!columns_query_.value.empty());
}

void Select::SetResultDefinitionFrom(
    const std::vector<SelectColumnData>& select_columns_data) {
  Expects(!select_columns_data.empty());

  result_definition_.value =
      select_columns_data |
      ranges::views::transform([](const SelectColumnData& column) {
        return ColumnType{.column = {column.name}, .type = column.type};
      }) |
      ranges::to_vector;

  Ensures(!result_definition_.value.empty());
}
}  // namespace stonks::sqldb::qb