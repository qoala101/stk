#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_

#include <string>
#include <vector>

#include "sqldb_qb_table_traits.h"
#include "sqldb_qb_types.h"
#include "sqldb_table_traits.h"
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
  template <typename Table,
            typename ColumnsTraits = ColumnsTraits<typename Table::Columns>>
  explicit Create(Table * /*unused*/)
      : Create{Table::GetName(), ColumnsTraits::GetCreateColumnsData(),
               ColumnsTraits::GetPrimaryKeysData(),
               ColumnsTraits::GetForeignKeysData()} {}

  /**
   * @brief Adds if-not-exists condition.
   */
  [[nodiscard]] auto IfNotExists() -> Create &;

  /**
   * @brief Builds the query from inputs.
   */
  [[nodiscard]] auto Build() const -> Query;

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
