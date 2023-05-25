/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLDB_QB_DELETE_H_
#define VH_SQLDB_QB_DELETE_H_

#include <string>

#include "cpp_meta_template_constructor.h"
#include "sqldb_concepts.h"  // IWYU pragma: keep
#include "sqldb_prm_types.h"
#include "sqldb_qb_wrapped_conditions.h"
#include "sqldb_types.h"

namespace vh::sqldb::qb {
/**
 * @brief Builds query which deletes rows from the table.
 */
class Delete {
 public:
  /**
   * @tparam Table Table to delete rows from.
   */
  template <TableDefinition Table>
  explicit Delete(cpp::meta::TemplateConstructor<Table> /*unused*/)
      : Delete{Table::GetName()} {}

  /**
   * @brief Adds condition to the query.
   */
  auto Where [[nodiscard]] (WhereCondition condition) -> Delete&;

  /**
   * @brief Builds the query.
   */
  auto Build [[nodiscard]] () const -> prm::Parametrized<Query>;

 private:
  explicit Delete(std::string table_name);

  Query table_name_{};
  prm::Parametrized<Query> where_query_{};
};
}  // namespace vh::sqldb::qb

#endif  // VH_SQLDB_QB_DELETE_H_
