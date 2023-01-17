#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_

#include <string>
#include <string_view>
#include <vector>

#include "cpp_lazy.h"
#include "cpp_meta_template_constructor.h"
#include "cpp_type_list.h"
#include "sqldb_concepts.h"
#include "sqldb_p_types.h"
#include "sqldb_qb_columns_traits.h"
#include "sqldb_qb_query_value.h"
#include "sqldb_qb_types.h"
#include "sqldb_qb_wrapped_conditions.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
/**
 * @brief Builds query to select data from DB.
 */
class Select {
 public:
  /**
   * @brief Query would select specified columns.
   */
  template <ColumnDefinition... Columns>
  explicit Select(cpp::meta::TemplateConstructor<Columns...> /*unused*/)
      : Select{
            ColumnsTraits<cpp::TypeList<Columns...>>::GetSelectColumnsData()} {}

  /**
   * @brief Query would select all table columns.
   */
  explicit Select(All);

  /**
   * @brief Query would result in 1 if result has any columns.
   */
  explicit Select(One);

  /**
   * @brief Specifies the main table to select from.
   */
  template <TableDefinition Table>
  auto From [[nodiscard]] () -> auto & {
    return From(Table::GetName(),
                cpp::Lazy<std::vector<SelectColumnData>>{[]() {
                  return ColumnsTraits<Table>::GetSelectColumnsData();
                }});
  }

  /**
   * @brief Joins the table on specified condition.
   */
  template <TableDefinition Table>
  auto Join [[nodiscard]] (const OnCondition &condition) -> auto & {
    return Join(Table::GetName(), condition);
  }

  /**
   * @brief Adds condition to the query.
   */
  auto Where [[nodiscard]] (WhereCondition condition) -> Select &;

  /**
   * @brief Specifies the ordering on column.
   */
  template <ColumnDefinition Column>
  auto OrderBy [[nodiscard]] (Order order) -> Select & {
    return OrderBy(Column::GetFullName(), order);
  }

  /**
   * @brief Specifies the limit of returned rows.
   */
  auto Limit [[nodiscard]] (const QueryValue &value) -> Select &;

  /**
   * @brief Builds the query.
   */
  auto Build [[nodiscard]] () const -> p::Parametrized<SelectQuery>;

 private:
  explicit Select(const std::vector<SelectColumnData> &select_columns_data);

  auto From [[nodiscard]] (
      std::string table_name,
      cpp::Lazy<std::vector<SelectColumnData>> select_columns_data) -> Select &;

  auto Join [[nodiscard]] (std::string_view table_name,
                           const OnCondition &condition) -> Select &;

  auto OrderBy [[nodiscard]] (std::string_view column_name,
                              Order order = Order::kAscending) -> Select &;

  void SetColumnsQueryFrom(
      const std::vector<SelectColumnData> &select_columns_data);

  void SetResultDefinitionFrom(
      const std::vector<SelectColumnData> &select_columns_data);

  bool select_all_{};
  Query table_name_{};
  Query columns_query_{};
  Query order_by_query_{};
  p::Parametrized<Query> where_query_{};
  p::Parametrized<Query> limit_query_{};
  p::Parametrized<Query> join_query_{};
  ResultDefinition result_definition_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_
