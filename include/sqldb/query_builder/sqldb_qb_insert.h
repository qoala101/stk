#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_INSERT_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_INSERT_H_

#include <function2/function2.hpp>

#include "sqldb_qb_common.h"
#include "sqldb_qb_query_value.h"
#include "sqldb_qb_table_traits.h"
#include "sqldb_table_traits.h"
#include "sqldb_types.h"
#include "sqldb_value.h"

namespace stonks::sqldb::qb {
/**
 * @brief Builds queries to insert data to the table.
 */
class Insert {
 public:
  Insert() = default;

  /**
   * @brief Query would insert data to all of the columns.
   */
  explicit Insert(All* /*unused*/);

  /**
   * @brief Specifies the value for the column.
   */
  template <typename Column>
  [[nodiscard]] auto Value(const QueryValue& value) -> auto& {
    return Value(ColumnTraits<Column>::GetName(), value);
  }

  /**
   * @brief Sets the table to insert to.
   */
  template <typename Table>
  [[nodiscard]] auto Into() -> auto& {
    return Into(TableTraits<Table>::GetName(), []() {
      return ColumnsTraits<typename Table::Columns>::GetNames();
    });
  }

  /**
   * @brief Builds the query.
   */
  [[nodiscard]] auto Build() const -> p::Parametrized<Query>;

 private:
  [[nodiscard]] auto Value(std::string_view column_name,
                           const QueryValue& value) -> Insert&;

  void ValueImpl(std::string_view column_name, const QueryValue& value);

  [[nodiscard]] auto Into(
      std::string table_name,
      const fu2::unique_function<std::vector<std::string>() const>&
          get_column_names) -> Insert&;

  bool insert_all_{};
  Query table_name_{};
  Query columns_query_{};
  p::Parametrized<Query> values_query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_INSERT_H_
