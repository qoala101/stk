#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_

#include <gsl/assert>
#include <string>
#include <string_view>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "sqldb_qb_common.h"
#include "sqldb_table_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
class Delete {
 public:
  template <typename Table>
  explicit Delete(Table* /*unused*/) : Delete{TableTraits<Table>::GetName()} {}

  [[nodiscard]] auto Where(const WhereQuery& where) -> Delete&;
  [[nodiscard]] auto And(const WhereQuery& where) -> Delete&;
  [[nodiscard]] auto Or(const WhereQuery& where) -> Delete&;

  [[nodiscard]] auto Build() const -> Query;

 private:
  explicit Delete(std::string table_name);

  std::string table_name_{};
  std::string where_clause_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_
