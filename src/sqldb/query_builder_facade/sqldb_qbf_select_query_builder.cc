#include "sqldb_qbf_select_query_builder.h"

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
#include "sqldb_qbf_common.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
auto SelectQueryBuilderTemplate::Where(std::string_view where_clause)
    -> SelectQueryBuilderTemplate & {
  Expects(where_clause_.empty());
  where_clause_ = fmt::format(" WHERE ({})", where_clause);
  Ensures(!where_clause_.empty());
  return *this;
}

auto SelectQueryBuilderTemplate::And(std::string_view where_clause)
    -> SelectQueryBuilderTemplate & {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" AND ({})", where_clause);
  return *this;
}
auto SelectQueryBuilderTemplate::Or(std::string_view where_clause)
    -> SelectQueryBuilderTemplate & {
  Expects(!where_clause_.empty());
  where_clause_ += fmt::format(" OR ({})", where_clause);
  return *this;
}

auto SelectQueryBuilderTemplate::Limit(int limit)
    -> SelectQueryBuilderTemplate & {
  Expects(std::holds_alternative<std::monostate>(limit_.value));
  limit_.value.emplace<int>(limit);
  Ensures(!std::holds_alternative<std::monostate>(limit_.value));
  return *this;
}

auto SelectQueryBuilderTemplate::LimitParam() -> SelectQueryBuilderTemplate & {
  Expects(std::holds_alternative<std::monostate>(limit_.value));
  limit_.value.emplace<LimitedType>();
  Ensures(!std::holds_alternative<std::monostate>(limit_.value));
  return *this;
}

auto SelectQueryBuilderTemplate::Build() const
    -> std::pair<Query, std::vector<CellDefinition>> {
  Expects(!table_name_.empty());
  Expects(!cell_definitions_.empty());

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

  return {{std::move(query)}, cell_definitions_};
}
}  // namespace stonks::sqldb::qbf