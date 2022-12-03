#include "sqldb_qb_update.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <not_null.hpp>
#include <range/v3/detail/variant.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/utility/get.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/concat.hpp>
#include <utility>
#include <vector>

#include "cpp_typed_struct.h"
#include "sqldb_p_types.h"

namespace stonks::sqldb::qb {
auto Update::Where(WhereCondition condition) -> Update& {
  Expects(where_query_->empty());

  auto& where_query = condition.GetQuery();
  Expects(!where_query->empty());

  where_query_ = std::move(where_query);
  Ensures(!where_query_->empty());
  return *this;
}

auto Update::Build() const -> p::Parametrized<Query> {
  Expects(!column_values_query_->empty());

  return {fmt::format("UPDATE {} SET {}{}", *table_name_, *column_values_query_,
                      *where_query_),
          ranges::views::concat(*column_values_query_.params,
                                *where_query_.params) |
              ranges::to_vector};
}

Update::Update(std::string table_name)
    : table_name_{[&table_name]() {
        Expects(!table_name.empty());
        return std::move(table_name);
      }()} {
  Ensures(!table_name_->empty());
}

auto Update::Set(std::string_view column_name, const QueryValue& value)
    -> Update& {
  const auto& value_query = value.GetQuery();
  Expects(!value_query->empty());

  if (!column_values_query_->empty()) {
    *column_values_query_ += ", ";
  }

  *column_values_query_ += fmt::format("{} = {}", column_name, *value_query);
  column_values_query_.params += value_query.params;

  Ensures(!column_values_query_->empty());
  return *this;
}
}  // namespace stonks::sqldb::qb