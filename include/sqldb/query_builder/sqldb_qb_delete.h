#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_

#include <string>

#include "cpp_template_constructor.h"
#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_p_types.h"
#include "sqldb_qb_wrapped_conditions.h"
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
  template <TableDefinition Table>
  explicit Delete(cpp::TemplateConstructor<Table> /*unused*/)
      : Delete{Table::GetName()} {}

  /**
   * @brief Adds condition to the query.
   */
  auto Where [[nodiscard]] (WhereCondition condition) -> Delete&;

  /**
   * @brief Builds the query.
   */
  auto Build [[nodiscard]] () const -> p::Parametrized<Query>;

 private:
  explicit Delete(std::string table_name);

  Query table_name_{};
  p::Parametrized<Query> where_query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_DELETE_H_
