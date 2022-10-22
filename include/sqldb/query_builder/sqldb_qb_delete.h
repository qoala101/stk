#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_

#include <gsl/assert>
#include <string>
#include <string_view>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
class Delete {
 public:
  template <typename Table>
  explicit Delete(Table * /*unused*/)
      : table_name_{TableTraits<Table>::GetName()} {
    Ensures(!table_name_.empty());
  }

  [[nodiscard]] auto Where(std::string_view where_clause) -> Delete &;
  [[nodiscard]] auto And(std::string_view where_clause) -> Delete &;
  [[nodiscard]] auto Or(std::string_view where_clause) -> Delete &;

  [[nodiscard]] auto Build() const -> Query;

 private:
  std::string table_name_{};
  std::string where_clause_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_
