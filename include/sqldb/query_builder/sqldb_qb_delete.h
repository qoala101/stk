#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_

#include <string>

#include "sqldb_qb_wrapped_conditions.h"
#include "sqldb_table_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
/**
 * @brief Builds query which deletes rows from the table.
 */
class Delete {
 public:
  /**
   * @tparam Table Table to delete rows from.
   */
  template <typename Table>
  explicit Delete(Table* /*unused*/) : Delete{Table::GetName()} {}

  /**
   * @brief Adds condition to the query.
   */
  [[nodiscard]] auto Where(WhereCondition condition) -> Delete&;

  /**
   * @brief Builds the query.
   */
  [[nodiscard]] auto Build() const -> p::Parametrized<Query>;

 private:
  explicit Delete(std::string table_name);

  Query table_name_{};
  p::Parametrized<Query> where_query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_
