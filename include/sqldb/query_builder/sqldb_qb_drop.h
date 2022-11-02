#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DROP_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DROP_H_

#include <string>

#include "sqldb_table_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
/**
 * @brief Builds drop table query.
 */
class Drop {
 public:
  /**
   * @tparam Table Table to drop.
   */
  template <typename Table>
  explicit Drop(Table* /*unused*/) : Drop{Table::GetName()} {}

  /**
   * @brief Builds the query.
   */
  [[nodiscard]] auto Build() const -> Query;

 private:
  explicit Drop(std::string table_name);

  Query table_name_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DROP_H_
