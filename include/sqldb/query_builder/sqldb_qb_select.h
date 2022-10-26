#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_

#include <function2/function2.hpp>
#include <string>
#include <string_view>
#include <vector>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_views.h"
#include "sqldb_qb_common.h"
#include "sqldb_qb_condition.h"
#include "sqldb_qb_table_traits.h"
#include "sqldb_qb_types.h"
#include "sqldb_table_traits.h"
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
  template <typename... Columns>
  explicit Select(std::tuple<Columns...> * /*unused*/)
      : Select{ColumnsTraits<std::tuple<Columns...>>::GetFullColumnTypes()} {}

  /**
   * @brief Query would select all table columns.
   */
  explicit Select(All * /*unused*/);

  /**
   * @brief Query would result in 1 if result has any columns.
   */
  explicit Select(One * /*unused*/);

  /**
   * @brief Specifies the main table to select from.
   */
  template <typename Table>
  [[nodiscard]] auto From() -> auto & {
    return From(TableTraits<Table>::GetName(), []() {
      return ColumnsTraits<typename Table::Columns>::GetFullColumnTypes();
    });
  }

  /**
   * @brief Joins the table on specified condition.
   */
  template <typename Table>
  [[nodiscard]] auto Join(const Condition &condition) -> auto & {
    return Join(TableTraits<Table>::GetName(), condition);
  }

  /**
   * @brief Adds condition to the query.
   */
  [[nodiscard]] auto Where(WhereCondition condition) -> Select &;

  /**
   * @brief Specifies the limit of returned rows.
   */
  [[nodiscard]] auto Limit(const QueryValue &value) -> Select &;

  /**
   * @brief Builds the query.
   */
  [[nodiscard]] auto Build() const -> SelectQuery;

 private:
  explicit Select(const std::vector<FullColumnType> &columns);

  [[nodiscard]] auto From(
      std::string table_name,
      const fu2::unique_function<std::vector<FullColumnType>() const>
          &get_columns) -> Select &;

  [[nodiscard]] auto Join(std::string_view table_name,
                          const Condition &condition) -> Select &;

  void SetColumnsQuery(const std::vector<FullColumnType> &columns);
  void SetResultDefinition(const std::vector<FullColumnType> &columns);

  bool select_all_{};
  std::string table_name_{};
  std::string columns_query_{};
  std::string where_query_{};
  std::string limit_query_{};
  std::string join_query_{};
  ResultDefinition result_definition_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_
