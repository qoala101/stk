#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_INSERT_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_INSERT_QUERY_BUILDER_H_

#include <vector>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_views.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_qbf_common.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
class QueryBuilderFacade;

namespace qbf {
/**
 * @brief Part of query builder facade.
 */
class InsertQueryBuilder {
 public:
  auto WholeRow() -> InsertQueryBuilder &;
  auto IntoTable(Table table) -> InsertQueryBuilder &;
  auto IntoTable(TableDefinition table_definition) -> InsertQueryBuilder &;
  auto IntoColumns(std::vector<Column> columns) -> InsertQueryBuilder &;
  auto IntoColumns(const cpp::ConstView<ColumnDefinition> &column_definitions)
      -> InsertQueryBuilder &;

  [[nodiscard]] auto Build() const -> Query;

 private:
  friend class cpp::ExposePrivateConstructorsTo<QueryBuilderFacade,
                                                InsertQueryBuilder>;

  explicit InsertQueryBuilder(cpp::NnSp<IQueryBuilder> query_builder);

  cpp::NnSp<IQueryBuilder> query_builder_;

  TableVariant table_{};
  ColumnsVariant columns_{};
};
}  // namespace qbf
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_INSERT_QUERY_BUILDER_H_
