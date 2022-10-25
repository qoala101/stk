#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_

#include <string>
#include <tuple>

#include "sqldb_qb_table_traits.h"
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
      : Create{TableTraits<Table>::GetName(), ColumnsTraits::GetDefinitions(),
               ColumnsTraits::GetPrimaryKeys(),
               ColumnsTraits::GetForeignKeys()} {}

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
         const std::vector<ColumnDefinition> &column_definitions,
         const std::vector<PrimaryKey> &primary_keys,
         const std::vector<ForeignKey> &foreign_keys);

  std::string table_name_{};
  std::string columns_query_{};
  std::string primary_keys_query_{};
  std::string foreign_keys_query_{};
  std::string if_not_exists_query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
