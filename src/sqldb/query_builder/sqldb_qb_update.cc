#include "sqldb_qb_update.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qb_common.h"

namespace stonks::sqldb::qb {
auto Update::Set(std::string_view column_name, const QueryValue& value)
    -> Update& {
  const auto& value_query = value.GetQuery();
  Expects(!value_query.empty());

  if (!column_values_query_.empty()) {
    column_values_query_ += ", ";
  }

  column_values_query_ += fmt::format("{} = {}", column_name, value_query);

  Ensures(!column_values_query_.empty());
  return *this;
}

auto Update::Where(WhereCondition condition) -> Update& {
  auto& where_query = condition.GetQuery();
  Expects(!where_query.empty());
  where_query_ = std::move(where_query);
  Ensures(!where_query_.empty());
  return *this;
}

auto Update::Build() const -> Query {
  Expects(!column_values_query_.empty());
  auto query = fmt::format("UPDATE {} SET {}{}", table_name_,
                           column_values_query_, where_query_);
  Ensures(!query.empty());
  return {std::move(query)};
}

Update::Update(std::string table_name)
    : table_name_{[&table_name]() {
        Expects(!table_name.empty());
        return std::move(table_name);
      }()} {
  Ensures(!table_name_.empty());
}
}  // namespace stonks::sqldb::qb