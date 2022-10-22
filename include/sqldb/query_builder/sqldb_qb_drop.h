#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DROP_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DROP_H_

#include <gsl/assert>
#include <string>
#include <string_view>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
class Drop {
 public:
  template <typename Table>
  explicit Drop(Table* /*unused*/)
      : table_name_{TableTraits<Table>::GetName()} {
    Ensures(!table_name_.empty());
  }

  [[nodiscard]] auto Build() const -> Query;

 private:
  std::string table_name_{};
  std::string if_not_exists_clause_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DROP_H_
