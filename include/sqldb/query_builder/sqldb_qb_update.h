#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_

#include <string>
#include <string_view>

#include "sqldb_qb_common.h"
#include "sqldb_table_traits.h"
#include "sqldb_types.h"
#include "sqldb_value.h"

namespace stonks::sqldb::qb {
/**
 * @brief Builds the query to update values in the table.
 */
class Update {
 public:
  template <typename Table>
  explicit Update(Table* /*unused*/) : Update{TableTraits<Table>::GetName()} {}

  /**
   * @brief Sets the value for the column.
   */
  template <typename Column>
  [[nodiscard]] auto Set(const QueryValue& value) -> auto& {
    return Set(::stonks::sqldb::ColumnTraits<Column>::GetName(), value);
  }

  /**
   * @brief Adds condition to the query.
   */
  [[nodiscard]] auto Where(WhereCondition condition) -> Update&;

  /**
   * @brief Builds the query.
   */
  [[nodiscard]] auto Build() const -> Query;

 private:
  explicit Update(std::string table_name);

  [[nodiscard]] auto Set(std::string_view column_name, const QueryValue& value)
      -> Update&;

  std::string table_name_{};
  std::string column_values_query_{};
  std::string where_query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_
