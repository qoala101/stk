#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_

#include <string>
#include <string_view>

#include "cpp_meta_template_constructor.h"
#include "sqldb_concepts.h"
#include "sqldb_prm_types.h"
#include "sqldb_qb_query_value.h"
#include "sqldb_qb_wrapped_conditions.h"
#include "sqldb_types.h"

namespace vh::sqldb::qb {
/**
 * @brief Builds the query to update values in the table.
 */
class Update {
 public:
  /**
   * @tparam Table Table to update.
   */
  template <TableDefinition Table>
  explicit Update(cpp::meta::TemplateConstructor<Table> /*unused*/)
      : Update{Table::GetName()} {}

  /**
   * @brief Sets the value for the column.
   */
  template <ColumnDefinition Column>
  auto Set [[nodiscard]] (const QueryValue& value) -> auto& {
    return Set(Column::GetName(), value);
  }

  /**
   * @brief Adds condition to the query.
   */
  auto Where [[nodiscard]] (WhereCondition condition) -> Update&;

  /**
   * @brief Builds the query.
   */
  auto Build [[nodiscard]] () const -> prm::Parametrized<Query>;

 private:
  explicit Update(std::string table_name);

  auto Set [[nodiscard]] (std::string_view column_name, const QueryValue& value)
  -> Update&;

  Query table_name_{};
  prm::Parametrized<Query> column_values_query_{};
  prm::Parametrized<Query> where_query_{};
};
}  // namespace vh::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_UPDATE_H_
