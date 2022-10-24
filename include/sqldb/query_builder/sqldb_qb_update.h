#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_

#include <string>
#include <string_view>
#include <vector>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_views.h"
#include "sqldb_table_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
class Update {
 public:
  template <typename Table>
  explicit Update(Table * /*unused*/)
      : table_name_{TableTraits<Table>::GetName()} {}

  template <typename Column>
  [[nodiscard]] auto Set(std::string TEMP) -> auto & {
    if (!column_values_.empty()) {
      column_values_ += ", ";
    }

    column_values_ += fmt::format("{} = {}", ColumnTraits<Column>::GetName(), TEMP);

    return *this;
  }

  [[nodiscard]] auto Where(std::string_view where_clause)
      -> Update &;
  [[nodiscard]] auto And(std::string_view where_clause)
      -> Update &;
  [[nodiscard]] auto Or(std::string_view where_clause)
      -> Update &;

  [[nodiscard]] auto Build() const -> Query;

 private:
  std::string table_name_{};
  std::string column_values_{};
  std::string where_clause_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_
