#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_

#include <string>
#include <string_view>

#include "sqldb_qb_query_value.h"
#include "sqldb_qb_wrapped_conditions.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
/**
 * @brief Builds the query to update values in the table.
 */
class Update {
 public:
  /**
   * @tparam Table Table to update.
   */
  template <TableDefinition Table>
  explicit Update(Table* /*unused*/) : Update{Table::GetName()} {}

  /**
   * @brief Sets the value for the column.
   */
  template <ColumnDefinition Column>
  [[nodiscard]] auto Set(const QueryValue& value) -> auto& {
    return Set(Column::GetName(), value);
  }

  /**
   * @brief Adds condition to the query.
   */
  [[nodiscard]] auto Where(WhereCondition condition) -> Update&;

  /**
   * @brief Builds the query.
   */
  [[nodiscard]] auto Build() const -> p::Parametrized<Query>;

 private:
  explicit Update(std::string table_name);

  [[nodiscard]] auto Set(std::string_view column_name, const QueryValue& value)
      -> Update&;

  Query table_name_{};
  p::Parametrized<Query> column_values_query_{};
  p::Parametrized<Query> where_query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_
