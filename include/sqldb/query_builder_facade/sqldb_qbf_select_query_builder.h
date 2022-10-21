#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_SELECT_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_SELECT_QUERY_BUILDER_H_

#include <string>
#include <string_view>
#include <vector>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_views.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_qbf_columns_variant.h"
#include "sqldb_qbf_limit_variant.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
class QueryBuilderFacade;

namespace qbf {
/**
 * @brief Part of query builder facade.
 */
class SelectQueryBuilder {
 public:
  auto Columns(std::vector<Column> columns) -> SelectQueryBuilder &;
  auto Columns(const cpp::ConstView<ColumnDefinition> &column_definitions)
      -> SelectQueryBuilder &;
  auto AllColumns() -> SelectQueryBuilder &;

  auto FromTable(Table table) -> SelectQueryBuilder &;
  auto FromTable(const TableDefinition &table_definition)
      -> SelectQueryBuilder &;

  auto Where(std::string_view where_clause) -> SelectQueryBuilder &;
  auto And(std::string_view where_clause) -> SelectQueryBuilder &;
  auto Or(std::string_view where_clause) -> SelectQueryBuilder &;

  auto Limit(int limit) -> SelectQueryBuilder &;
  auto Limited() -> SelectQueryBuilder &;

  [[nodiscard]] auto Build() const -> Query;

 private:
  friend class cpp::ExposePrivateConstructorsTo<QueryBuilderFacade,
                                                SelectQueryBuilder>;

  explicit SelectQueryBuilder(cpp::NnUp<IQueryBuilder> query_builder);

  cpp::NnUp<IQueryBuilder> query_builder_;

  cpp::Opt<Table> table_{};
  ColumnsVariant columns_{};
  cpp::Opt<std::string> where_clause_{};
  LimitVariant limit_{};
};

struct All;
struct One;

class SelectQueryBuilderTemplate {
 public:
  template <typename... Columns>
  explicit SelectQueryBuilderTemplate(std::tuple<Columns...> * /*unused*/)
      : cell_definitions_{ColumnsTraits<std::tuple<Columns...>>::GetTypes()},
        full_column_names_{
            ColumnsTraits<std::tuple<Columns...>>::GetFullNames()} {}

  explicit SelectQueryBuilderTemplate(All * /*unused*/) : select_all_{true} {}

  explicit SelectQueryBuilderTemplate(One * /*unused*/) : select_one_{true} {}

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

  [[nodiscard]] auto Where(std::string_view where_clause)
      -> SelectQueryBuilderTemplate &;
  [[nodiscard]] auto And(std::string_view where_clause)
      -> SelectQueryBuilderTemplate &;
  [[nodiscard]] auto Or(std::string_view where_clause)
      -> SelectQueryBuilderTemplate &;

  template <typename Table>
  [[nodiscard]] auto Join(std::string_view on_clause) -> auto & {
    join_clause_ +=
        fmt::format(" JOIN {} {}", TableTraits<Table>::GetName(), on_clause);

    Ensures(!join_clause_.empty());
    return *this;
  }

  [[nodiscard]] auto Limit(int limit) -> SelectQueryBuilderTemplate &;

  [[nodiscard]] auto LimitParam() -> SelectQueryBuilderTemplate &;

  [[nodiscard]] auto Build() const
      -> std::pair<Query, std::vector<CellDefinition>>;

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
}  // namespace qbf
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_SELECT_QUERY_BUILDER_H_
