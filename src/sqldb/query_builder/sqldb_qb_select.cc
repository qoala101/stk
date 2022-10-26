#include "sqldb_qb_select.h"

#include <absl/strings/str_join.h>
#include <fmt/core.h>

#include <gsl/assert>
#include <iostream>
#include <memory>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qb_common.h"
#include "sqldb_qb_types.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
Select::Select(All* /*unused*/) : select_all_{true}, columns_query_{"*"} {
  Ensures(select_all_);
  Ensures(!columns_query_.empty());
}

Select::Select(One* /*unused*/)
    : columns_query_{"1"},
      result_definition_{{{{.column = {"1"}, .type = {DataType<int>{}}}}}} {
  Ensures(!columns_query_.empty());
  Ensures(!result_definition_.value.empty());
}

auto Select::Where(WhereCondition condition) -> Select& {
  auto& where_query = condition.GetQuery();
  Expects(!where_query.empty());
  where_query_ = std::move(where_query);
  Ensures(!where_query_.empty());
  return *this;
}

auto Select::Limit(const QueryValue& value) -> Select& {
  const auto& value_query = value.GetQuery();
  Expects(!value_query.empty());
  limit_query_ = fmt::format(" LIMIT {}", value.GetQuery());
  Ensures(!limit_query_.empty());
  return *this;
}

auto Select::Build() const -> SelectQuery {
  Expects(!table_name_.empty());
  Expects(!columns_query_.empty());

  auto query =
      fmt::format("SELECT {} FROM {}{}{}{}", columns_query_, table_name_,
                  join_query_, where_query_, limit_query_);

  Ensures(!query.empty());
  return {std::move(query), result_definition_};
}

Select::Select(const std::vector<FullColumnType>& columns) {
  SetColumnsQuery(columns);
  SetResultDefinition(columns);
}

auto Select::From(std::string table_name,
                  const fu2::unique_function<std::vector<FullColumnType>()
                                                 const>& get_columns)
    -> Select& {
  Expects(table_name_.empty());
  Expects(!table_name.empty());
  table_name_ = std::move(table_name);

  if (select_all_) {
    SetResultDefinition(get_columns());
  }

  Ensures(!table_name_.empty());
  Ensures(!result_definition_.value.empty());
  return *this;
}

auto Select::Join(std::string_view table_name, const Condition& condition)
    -> Select& {
  join_query_ +=
      fmt::format(" JOIN {} ON ({})", table_name, condition.GetQuery());

  Ensures(!join_query_.empty());
  return *this;
}

void Select::SetColumnsQuery(const std::vector<FullColumnType>& columns) {
  Expects(!columns.empty());

  columns_query_ = absl::StrJoin(
      columns | ranges::views::transform([](const FullColumnType& column) {
        return column.full_name;
      }),
      ", ");

  Ensures(!columns_query_.empty());
}

void Select::SetResultDefinition(const std::vector<FullColumnType>& columns) {
  Expects(!columns.empty());

  result_definition_.value =
      columns | ranges::views::transform([](const FullColumnType& column) {
        return ColumnType{.column = {column.name}, .type = column.type};
      }) |
      ranges::to_vector;

  Ensures(!result_definition_.value.empty());
}
}  // namespace stonks::sqldb::qb