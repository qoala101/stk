#include "sqldb_qb_update.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/concat.hpp>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_p_types.h"

namespace stonks::sqldb::qb {
auto Update::Where(WhereCondition condition) -> Update& {
  auto& where_query = condition.GetQuery();
  Expects(!where_query.value.empty());
  where_query_ = std::move(where_query);
  Ensures(!where_query_.value.empty());
  return *this;
}

auto Update::Build() const -> p::Parametrized<Query> {
  Expects(!column_values_query_.value.empty());

  return {fmt::format("UPDATE {} SET {}{}", table_name_.value,
                      column_values_query_.value, where_query_.value),
          ranges::views::concat(column_values_query_.params.value,
                                where_query_.params.value) |
              ranges::to_vector};
}

Update::Update(std::string table_name)
    : table_name_{[&table_name]() {
        Expects(!table_name.empty());
        return std::move(table_name);
      }()} {
  Ensures(!table_name_.value.empty());
}

auto Update::Set(std::string_view column_name, const QueryValue& value)
    -> Update& {
  const auto& value_query = value.GetQuery();
  Expects(!value_query.value.empty());

  if (!column_values_query_.value.empty()) {
    column_values_query_.value += ", ";
  }

  column_values_query_.value +=
      fmt::format("{} = {}", column_name, value_query.value);
  column_values_query_.params += value_query.params;

  Ensures(!column_values_query_.value.empty());
  return *this;
}
}  // namespace stonks::sqldb::qb