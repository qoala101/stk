#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_INSERT_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_INSERT_H_

#include "sqldb_qb_common.h"
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
  [[nodiscard]] auto Value(const Value& value) -> auto& {
    return Value(ColumnTraits<Column>::GetName(), value);
  }

  /**
   * @brief Specifies the param for the column to be substituted later.
   */
  template <typename Column>
  [[nodiscard]] auto Value(const Param& param) -> auto& {
    return Value(ColumnTraits<Column>::GetName(), param);
  }

  /**
   * @brief Sets the table to insert to.
   */
  template <typename Table>
  [[nodiscard]] auto Into() -> auto& {
    auto table_name = TableTraits<Table>::GetName();

    if (insert_all_) {
      return Into(std::move(table_name),
                  ColumnsTraits<typename Table::Columns>::GetNames());
    }

    return Into(std::move(table_name));
  }

  /**
   * @brief Builds the query.
   */
  [[nodiscard]] auto Build() const -> Query;

 private:
  [[nodiscard]] auto Value(std::string_view column_name, std::string_view value)
      -> auto&;
  [[nodiscard]] auto Value(std::string_view column_name,
                           const class Value& value) -> Insert&;
  [[nodiscard]] auto Value(std::string_view column_name, const Param& param)
      -> Insert&;

  [[nodiscard]] auto Into(std::string table_name) -> Insert&;
  [[nodiscard]] auto Into(std::string table_name,
                          const std::vector<std::string>& column_names)
      -> Insert&;

  bool insert_all_{};
  std::string table_name_{};
  std::string columns_query_{};
  std::string values_query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_INSERT_H_
