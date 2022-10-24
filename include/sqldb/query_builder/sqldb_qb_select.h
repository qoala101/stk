#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_

#include <string>
#include <string_view>
#include <vector>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_views.h"
#include "sqldb_qb_common.h"
#include "sqldb_qb_limit_variant.h"
#include "sqldb_table_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
namespace detail {
struct ColumnType {
  std::string name{};
  DataTypeVariant type{};
};
}  // namespace detail

struct All;
struct One;

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
      // : Select{ColumnsTraits<std::tuple<Columns...>>::GetNames()} {}
      : cell_definitions_{ColumnsTraits<std::tuple<Columns...>>::GetTypes()},
        full_column_names_{
            ColumnsTraits<std::tuple<Columns...>>::GetFullNames()} {}

  /**
   * @brief Query would select all table columns.
   */
  explicit Select(All * /*unused*/);

  /**
   * @brief Query would result in 1 if result has any columns.
   */
  explicit Select(One * /*unused*/);

  template <typename Table>
  [[nodiscard]] auto From() -> auto & {
    Expects(table_name_.empty());
    table_name_ = TableTraits<Table>::GetName();

    if (cell_definitions_.value.empty()) {
      cell_definitions_ = {ColumnsTraits<typename Table::Columns>::GetTypes()};
    }

    if (const auto full_column_names_required =
            !select_all_ && !select_one_ && full_column_names_.empty()) {
      full_column_names_ =
          ColumnsTraits<typename Table::Columns>::GetFullNames();
    }

    Ensures(!table_name_.empty());
    Ensures(!cell_definitions_.value.empty());
    return *this;
  }

  [[nodiscard]] auto Where(const WhereQuery &where) -> Select &;
  [[nodiscard]] auto And(const WhereQuery &where) -> Select &;
  [[nodiscard]] auto Or(const WhereQuery &where) -> Select &;

  template <typename Table>
  [[nodiscard]] auto Join(std::string_view on_clause) -> auto & {
    join_clause_ +=
        fmt::format(" JOIN {} {}", TableTraits<Table>::GetName(), on_clause);

    Ensures(!join_clause_.empty());
    return *this;
  }

  [[nodiscard]] auto Limit(int limit) -> Select &;

  [[nodiscard]] auto LimitParam() -> Select &;

  [[nodiscard]] auto Build() const -> SelectQuery;

 private:
  explicit Select(const std::vector<std::string> &column_names);

  bool select_all_{};
  bool select_one_{};
  std::string table_name_{};
  ResultDefinition cell_definitions_{};
  std::string full_column_names_{};
  std::string where_clause_{};
  LimitVariant limit_{};

  std::string join_clause_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_
