#include "sqldb_qbf_insert_query_builder.h"

#include <gsl/assert>
#include <memory>
#include <utility>
#include <variant>

#include "cpp_typed_struct.h"
#include "sqldb_qbf_columns_variant.h"
#include "sqldb_qbf_common.h"

namespace stonks::sqldb::qbf {
auto InsertQueryBuilderTemplate::Build() const -> Query {
  Expects(!table_name_.empty());

  auto query = fmt::format("INSERT INTO {} ({}) VALUES ({})", table_name_,
                           column_names_, values_);

  Ensures(!query.empty());
  return {std::move(query)};
}
}  // namespace stonks::sqldb::qbf