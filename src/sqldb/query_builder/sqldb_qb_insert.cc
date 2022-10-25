#include "sqldb_qb_insert.h"

#include <absl/strings/str_cat.h>
#include <absl/strings/str_join.h>
#include <absl/strings/string_view.h>
#include <fmt/core.h>

#include <gsl/assert>
#include <gsl/util>
#include <memory>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop.hpp>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qb_common.h"
#include "sqldb_value.h"

namespace stonks::sqldb::qb {
Insert::Insert(All* /*unused*/) : insert_all_{true} {}

auto Insert::Value(std::string_view column_name, const QueryValue& value)
    -> Insert& {
  Expects(!column_name.empty());

  const auto& value_query = value.GetQuery();
  Expects(!value_query.empty());

  if (!columns_query_.empty()) {
    columns_query_ += ", ";
  }

  columns_query_ += column_name;

  if (!values_query_.empty()) {
    values_query_ += ", ";
  }

  values_query_ += value_query;

  Ensures(!columns_query_.empty());
  Ensures(!values_query_.empty());
  return *this;
}

auto Insert::Into(std::string table_name) -> Insert& {
  Expects(table_name_.empty());
  Expects(!table_name.empty());
  table_name_ = std::move(table_name);
  Ensures(!table_name_.empty());
  return *this;
}

auto Insert::Into(std::string table_name,
                  const std::vector<std::string>& column_names) -> Insert& {
  const auto value = QueryValue{Param{}};

  for (const auto& column_name : column_names) {
    std::ignore = Value(column_name, value);
  }

  return Into(std::move(table_name));
}

auto Insert::Build() const -> Query {
  Expects(!table_name_.empty());
  Expects(!columns_query_.empty());
  Expects(!values_query_.empty());

  auto query = fmt::format("INSERT INTO {} ({}) VALUES ({})", table_name_,
                           columns_query_, values_query_);

  Ensures(!query.empty());
  return {std::move(query)};
}
}  // namespace stonks::sqldb::qb