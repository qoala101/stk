#include "sqldb_qb_update.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <memory>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qb_common.h"

namespace stonks::sqldb::qb {
auto Update::Set(std::string_view column_name, std::string_view value)
    -> auto& {
  if (!column_values_query_.empty()) {
    column_values_query_ += ", ";
  }

  column_values_query_ += fmt::format("{} = {}", column_name, value);

  Ensures(!column_values_query_.empty());
  return *this;
}

auto Update::Set(std::string_view column_name, const class Value& value)
    -> Update& {
  return Set(column_name, value.ToString());
}

auto Update::Set(std::string_view column_name, const Param& param) -> Update& {
  return Set(column_name, param.text_);
}

auto Update::Where(const WhereQuery& where) -> Update& {
  Expects(where_query_.empty());
  where_query_ = fmt::format(" WHERE ({})", where.value);
  Ensures(!where_query_.empty());
  return *this;
}

auto Update::And(const WhereQuery& where) -> Update& {
  Expects(!where_query_.empty());
  where_query_ += fmt::format(" AND ({})", where.value);
  return *this;
}
auto Update::Or(const WhereQuery& where) -> Update& {
  Expects(!where_query_.empty());
  where_query_ += fmt::format(" OR ({})", where.value);
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