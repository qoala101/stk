#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_

#include <string>
#include <string_view>
#include <vector>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_views.h"
#include "sqldb_qb_limit_variant.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
struct All;
struct One;

class Select {
 public:
  template <typename... Columns>
  explicit Select(std::tuple<Columns...> * /*unused*/)
      : cell_definitions_{ColumnsTraits<std::tuple<Columns...>>::GetTypes()},
        full_column_names_{
            ColumnsTraits<std::tuple<Columns...>>::GetFullNames()} {}

  explicit Select(All * /*unused*/) : select_all_{true} {}

  explicit Select(One * /*unused*/) : select_one_{true} {}

  template <typename Table>
  [[nodiscard]] auto From() -> auto & {
    Expects(table_name_.empty());
    table_name_ = TableTraits<Table>::GetName();

    if (cell_definitions_.empty()) {
      cell_definitions_ = ColumnsTraits<typename Table::Columns>::GetTypes();
    }

    if (const auto full_column_names_required =
            !select_all_ && !select_one_ && full_column_names_.empty()) {
      full_column_names_ =
          ColumnsTraits<typename Table::Columns>::GetFullNames();
    }

    Ensures(!table_name_.empty());
    Ensures(!cell_definitions_.empty());
    return *this;
  }

  [[nodiscard]] auto Where(std::string_view where_clause) -> Select &;
  [[nodiscard]] auto And(std::string_view where_clause) -> Select &;
  [[nodiscard]] auto Or(std::string_view where_clause) -> Select &;

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
  bool select_all_{};
  bool select_one_{};
  std::string table_name_{};
  std::vector<CellDefinition> cell_definitions_{};
  std::string full_column_names_{};
  std::string where_clause_{};
  LimitVariant limit_{};

  std::string join_clause_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_SELECT_H_
