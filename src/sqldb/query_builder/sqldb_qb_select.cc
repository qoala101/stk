#include "sqldb_qb_select.h"

#include <absl/strings/str_join.h>
#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <range/v3/detail/variant.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/utility/get.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>

#include "cpp_typed_struct.h"
#include "sqldb_p_types.h"
#include "sqldb_qb_types.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
Select::Select(All /*unused*/) : select_all_{true}, columns_query_{"*"} {
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
  const auto& value_query = value.GetQuery();
  Expects(!value_query->empty());

  *limit_query_ = fmt::format(" LIMIT {}", *value_query);
  limit_query_.params = value_query.params;
  Ensures(!limit_query_->empty());
  return *this;
}

auto Select::Build() const -> p::Parametrized<SelectQuery> {
  Expects(!table_name_->empty());
  Expects(!columns_query_->empty());

  auto query =
      fmt::format("SELECT {} FROM {}{}{}{}", *columns_query_, *table_name_,
                  *join_query_, *where_query_, *limit_query_);
  auto params = ranges::views::concat(*join_query_.params, *where_query_.params,
                                      *limit_query_.params) |
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
  Expects(table_name_->empty());
  Expects(!table_name.empty());
  *table_name_ = std::move(table_name);

  if (select_all_) {
    SetResultDefinitionFrom(*select_columns_data);
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

void Select::SetColumnsQueryFrom(
    const std::vector<SelectColumnData>& select_columns_data) {
  Expects(!select_columns_data.empty());

  *columns_query_ = absl::StrJoin(
      select_columns_data | ranges::views::transform([](const auto& column) {
        return column.full_name;
      }),
      ", ");

  Ensures(!columns_query_->empty());
}

void Select::SetResultDefinitionFrom(
    const std::vector<SelectColumnData>& select_columns_data) {
  Expects(!select_columns_data.empty());

  *result_definition_ =
      select_columns_data | ranges::views::transform([](const auto& column) {
        return ColumnType{.column = {column.name}, .type = column.type};
      }) |
      ranges::to_vector;

  Ensures(!result_definition_->empty());
}
}  // namespace stonks::sqldb::qb