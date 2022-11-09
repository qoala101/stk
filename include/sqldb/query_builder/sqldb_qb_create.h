#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_

#include <string>
#include <vector>

#include "cpp_template_constructor.h"
#include "sqldb_qb_columns_traits.h"
#include "sqldb_qb_types.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
/**
 * @brief Builds create table query.
 */
class Create {
 public:
  /**
   * @tparam Table Table definition.
   */
  template <TableDefinition Table,
            typename ColumnsTraits = ColumnsTraits<Table>>
  explicit Create(cpp::TemplateConstructor<Table> /*unused*/)
      : Create{Table::GetName(), ColumnsTraits::GetCreateColumnsData(),
               ColumnsTraits::GetPrimaryKeysData(),
               ColumnsTraits::GetForeignKeysData()} {}

  /**
   * @brief Adds if-not-exists condition.
   */
  auto IfNotExists [[nodiscard]] () -> Create &;

  /**
   * @brief Builds the query from inputs.
   */
  auto Build [[nodiscard]] () const -> Query;

 private:
  Create(std::string table_name,
         const std::vector<CreateColumnData> &create_columns_data,
         const std::vector<PrimaryKeyData> &primary_keys_data,
         const std::vector<ForeignKeyData> &foreign_keys_data);

  Query table_name_{};
  Query columns_query_{};
  Query primary_keys_query_{};
  Query foreign_keys_query_{};
  Query if_not_exists_query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
