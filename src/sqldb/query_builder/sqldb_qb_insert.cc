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
namespace {
[[nodiscard]] auto ToString(const Value& value) {
  return std::visit(
      [&value](auto v) {
        using V = typename decltype(v)::Type;

        if constexpr (std::is_same_v<V, std::string>) {
          return fmt::format(R"("{}")", value.Get<V>());
        } else {
          return fmt::format("{}", value.Get<V>());
        }
      },
      value.GetType().value);
}
}  // namespace

Insert::Insert(All* /*unused*/) : insert_all_{true} {}

auto Insert::Value(std::string_view column_name, std::string_view value)
    -> auto& {
  Expects(!column_name.empty());
  Expects(!value.empty());

  if (!columns_query_.empty()) {
    columns_query_ += ", ";
  }

  columns_query_ += column_name;

  if (!values_query_.empty()) {
    values_query_ += ", ";
  }

  values_query_ += value;

  Ensures(!columns_query_.empty());
  Ensures(!values_query_.empty());
  return *this;
}

auto Insert::Value(std::string_view column_name, const class Value& value)
    -> Insert& {
  return Value(column_name, ToString(value));
}

auto Insert::Value(std::string_view column_name, const Param& param)
    -> Insert& {
  return Value(column_name, param.text_);
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
  const auto param = Param{};

  for (const auto& column_name : column_names) {
    std::ignore = Value(column_name, param);
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