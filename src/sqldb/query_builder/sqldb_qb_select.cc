#include "sqldb_qb_select.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <iostream>
#include <memory>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qb_common.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
Select::Select(All* /*unused*/) : select_all_{true} {}
Select::Select(One* /*unused*/) : select_one_{true} {}

auto Select::Where(const WhereQuery& where) -> Select& {
  Expects(where_clause_.empty());
  where_clause_ = fmt::format(" WHERE ({})", where.value);
  Ensures(!where_clause_.empty());
  return *this;
}

auto Select::And(const WhereQuery& where) -> Select& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" AND ({})", where.value);
  return *this;
}
auto Select::Or(const WhereQuery& where) -> Select& {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" OR ({})", where.value);
  return *this;
}

auto Select::Limit(int limit) -> Select& {
  Expects(std::holds_alternative<std::monostate>(limit_.value));
  limit_.value.emplace<int>(limit);
  Ensures(!std::holds_alternative<std::monostate>(limit_.value));
  return *this;
}

auto Select::LimitParam() -> Select& {
  Expects(std::holds_alternative<std::monostate>(limit_.value));
  limit_.value.emplace<LimitedType>();
  Ensures(!std::holds_alternative<std::monostate>(limit_.value));
  return *this;
}

auto Select::Build() const -> SelectQuery {
  Expects(!table_name_.empty());
  Expects(!cell_definitions_.value.empty());

  auto query = std::string{"SELECT "};

  if (select_all_) {
    query += "*";
  } else if (select_one_) {
    query += "1";
  } else {
    Expects(!full_column_names_.empty());
    query += full_column_names_;
  }

  query += fmt::format(" FROM {}{}{}{}", table_name_, join_clause_,
                       where_clause_, limit_.GetLimitClause());

  return {std::move(query), cell_definitions_};
}
}  // namespace stonks::sqldb::qb