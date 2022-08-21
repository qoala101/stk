#ifndef STONKS_SQLDB_QUERY_BUILDERS_SQLDB_SELECT_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDERS_SQLDB_SELECT_QUERY_BUILDER_H_

#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "ccutils_expose_private_constructors.h"
#include "ccutils_not_null.h"
#include "ccutils_views.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_query_builders_common.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
class QueryBuilderFacade;

namespace query_builder_facade {
/**
 * @brief Part of query builder facade.
 */
class SelectQueryBuilder {
 public:
  auto Columns(std::vector<Column> columns) -> SelectQueryBuilder &;
  auto Columns(const ccutils::ConstView<ColumnDefinition> &column_definitions)
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

  [[nodiscard]] auto Build() const -> std::string;

 private:
  friend class ccutils::ExposePrivateConstructorsTo<QueryBuilderFacade,
                                                    SelectQueryBuilder>;

  struct LimitedType {};

  using LimitVariant = std::variant<std::monostate, int, LimitedType>;

  explicit SelectQueryBuilder(ccutils::NnSp<IQueryBuilder> query_builder);

  ccutils::NnSp<IQueryBuilder> query_builder_;

  std::optional<Table> table_{};
  ColumnsVariant columns_{};
  std::optional<std::string> where_clause_{};
  LimitVariant limit_{};
};
}  // namespace query_builder_facade
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDERS_SQLDB_SELECT_QUERY_BUILDER_H_
