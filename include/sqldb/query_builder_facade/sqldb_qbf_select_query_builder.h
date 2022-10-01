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
}  // namespace qbf
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_SELECT_QUERY_BUILDER_H_
