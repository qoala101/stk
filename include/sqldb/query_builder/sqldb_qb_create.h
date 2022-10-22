#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_

#include <gsl/assert>
#include <string>
#include <string_view>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
class Create {
 public:
  template <typename Table>
  explicit Create(Table * /*unused*/)
      : table_definition{TableTraits<Table>::GetDefinition()} {}

  [[nodiscard]] auto IfNotExists() -> Create &;

  [[nodiscard]] auto Build() const -> Query;

 private:
  TableDefinition table_definition{};
  std::string if_not_exists_clause_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
