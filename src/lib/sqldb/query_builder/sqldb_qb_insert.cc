#include "sqldb_qb_insert.h"

#include <fmt/core.h>

#include <gsl/assert>
#include <not_null.hpp>
#include <string_view>
#include <utility>

#include "cpp_typed_struct.h"
#include "sqldb_prm_types.h"

namespace stonks::sqldb::qb {
Insert::Insert(All /*unused*/) : insert_all_{true} {}

auto Insert::Build() const -> prm::Parametrized<Query> {
  Expects(!table_name_->empty());
  Expects(!columns_query_->empty());
  Expects(!values_query_->empty());

  return {fmt::format("INSERT INTO {} ({}) VALUES ({})", *table_name_,
                      *columns_query_, *values_query_),
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
  Expects(!value_query->empty());

  if (!columns_query_->empty()) {
    *columns_query_ += ", ";
  }

  *columns_query_ += column_name;

  if (!values_query_->empty()) {
    *values_query_ += ", ";
  }

  *values_query_ += value_query;
  values_query_.params += value_query.params;

  Ensures(!columns_query_->empty());
  Ensures(!values_query_->empty());
}

auto Insert::Into(std::string table_name,
                  cpp::Lazy<std::vector<std::string>> column_names) -> Insert& {
  Expects(table_name_->empty());
  Expects(!table_name.empty());
  *table_name_ = std::move(table_name);

  if (insert_all_) {
    Expects(columns_query_->empty());
    Expects(values_query_->empty());

    const auto param = QueryValue{prm::QueryParam{}};

    for (const auto& column_name : *column_names) {
      ValueImpl(column_name, param);
    }
  }

  Ensures(!table_name_->empty());
  return *this;
}
}  // namespace stonks::sqldb::qb