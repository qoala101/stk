#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_DROP_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_DROP_QUERY_BUILDER_H_

#include <gsl/assert>
#include <string>
#include <string_view>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qbf {
class DropQueryBuilderTemplate {
 public:
  template <typename Table>
  [[nodiscard]] auto Table() -> auto& {
    Expects(table_name_.empty());
    table_name_ = TableTraits<Table>::GetName();
    Ensures(!table_name_.empty());
    return *this;
  }

  [[nodiscard]] auto Build() const -> Query;

 private:
  std::string table_name_{};
  std::string if_not_exists_clause_{};
};
}  // namespace stonks::sqldb::qbf

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_DROP_QUERY_BUILDER_H_
