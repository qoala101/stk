/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_SQLDB_QB_INSERT_H_
#define VH_SQLDB_QB_INSERT_H_

#include <string>
#include <string_view>
#include <vector>

#include "cpp_lazy.h"
#include "sqldb_concepts.h"
#include "sqldb_prm_types.h"
#include "sqldb_qb_columns_traits.h"
#include "sqldb_qb_query_value.h"
#include "sqldb_qb_types.h"
#include "sqldb_types.h"

namespace vh::sqldb::qb {
/**
 * @brief Builds queries to insert data to the table.
 */
class Insert {
 public:
  Insert() = default;

  /**
   * @brief Query would insert data to all of the columns.
   */
  explicit Insert(All);

  /**
   * @brief Specifies the value for the column.
   */
  template <ColumnDefinition Column>
  auto Value [[nodiscard]] (const QueryValue& value) -> auto& {
    return Value(Column::GetName(), value);
  }

  /**
   * @brief Sets the table to insert to.
   */
  template <TableDefinition Table>
  auto Into [[nodiscard]] () -> auto& {
    return Into(Table::GetName(), cpp::Lazy<std::vector<std::string>>{[]() {
                  return ColumnsTraits<Table>::GetNames();
                }});
  }

  /**
   * @brief Builds the query.
   */
  auto Build [[nodiscard]] () const -> prm::Parametrized<Query>;

 private:
  auto Value [[nodiscard]] (std::string_view column_name,
                            const QueryValue& value) -> Insert&;

  void ValueImpl(std::string_view column_name, const QueryValue& value);

  auto Into [[nodiscard]] (std::string table_name,
                           cpp::Lazy<std::vector<std::string>> column_names)
  -> Insert&;

  bool insert_all_{};
  Query table_name_{};
  Query columns_query_{};
  prm::Parametrized<Query> values_query_{};
};
}  // namespace vh::sqldb::qb

#endif  // VH_SQLDB_QB_INSERT_H_
