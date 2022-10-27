#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_

#include <function2/function2.hpp>
#include <string>
#include <string_view>
#include <vector>

#include "cpp_lazy.h"
#include "sqldb_qb_common.h"
#include "sqldb_qb_condition.h"
#include "sqldb_qb_on_condition.h"
#include "sqldb_qb_query_value.h"
#include "sqldb_qb_table_traits.h"
#include "sqldb_qb_types.h"
#include "sqldb_qb_where_condition.h"
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
    return From(
        TableTraits<Table>::GetName(),
        cpp::Lazy<std::vector<FullColumnType>>{[]() {
          return ColumnsTraits<typename Table::Columns>::GetFullColumnTypes();
        }});
  }

  /**
   * @brief Joins the table on specified condition.
   */
  template <typename Table>
  [[nodiscard]] auto Join(const OnCondition &condition) -> auto & {
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
  [[nodiscard]] auto Build() const -> p::Parametrized<SelectQuery>;

 private:
  explicit Select(const std::vector<FullColumnType> &columns);

  [[nodiscard]] auto From(std::string table_name,
                          const cpp::Lazy<std::vector<FullColumnType>> &columns)
      -> Select &;

  [[nodiscard]] auto Join(std::string_view table_name,
                          const OnCondition &condition) -> Select &;

  void SetColumnsQueryFrom(const std::vector<FullColumnType> &columns);

  void SetResultDefinitionFrom(const std::vector<FullColumnType> &columns);

  bool select_all_{};
  Query table_name_{};
  Query columns_query_{};
  p::Parametrized<Query> where_query_{};
  p::Parametrized<Query> limit_query_{};
  p::Parametrized<Query> join_query_{};
  ResultDefinition result_definition_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_
