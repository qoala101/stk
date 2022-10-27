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
#include "sqldb_p_types.h"
#include "sqldb_qb_common.h"

namespace stonks::sqldb::qb {
Insert::Insert(All* /*unused*/) : insert_all_{true} {}

auto Insert::Build() const -> p::Parametrized<Query> {
  Expects(!table_name_.value.empty());
  Expects(!columns_query_.value.empty());
  Expects(!values_query_.value.empty());

  return {fmt::format("INSERT INTO {} ({}) VALUES ({})", table_name_.value,
                      columns_query_.value, values_query_.value),
          values_query_.params};
}

auto Insert::Value(std::string_view column_name, const QueryValue& value)
    -> Insert& {
  Expects(!insert_all_);
  ValueImpl(column_name, value);
  return *this;
}

void Insert::ValueImpl(std::string_view column_name, const QueryValue& value) {
  Expects(!column_name.empty());

  const auto& value_query = value.GetQuery();
  Expects(!value_query.value.empty());

  if (!columns_query_.value.empty()) {
    columns_query_.value += ", ";
  }

  columns_query_.value += column_name;

  if (!values_query_.value.empty()) {
    values_query_.value += ", ";
  }

  values_query_.value += value_query.value;

  values_query_.params.Append(value_query.params);

  Ensures(!columns_query_.value.empty());
  Ensures(!values_query_.value.empty());
}

auto Insert::Into(std::string table_name,
                  const fu2::unique_function<std::vector<std::string>() const>&
                      get_column_names) -> Insert& {
  Expects(table_name_.value.empty());
  Expects(!table_name.empty());
  table_name_.value = std::move(table_name);

  if (insert_all_) {
    Expects(columns_query_.value.empty());
    Expects(values_query_.value.empty());

    const auto param = QueryValue{p::Param{}};

    for (const auto& column_name : get_column_names()) {
      ValueImpl(column_name, param);
    }
  }

  Ensures(!table_name_.value.empty());
  return *this;
}
}  // namespace stonks::sqldb::qb