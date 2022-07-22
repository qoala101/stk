#ifndef STONKS_SQLDB_QUERY_BUILDERS_SQLDB_INSERT_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDERS_SQLDB_INSERT_QUERY_BUILDER_H_

#include <memory>
#include <string>
#include <vector>

#include "ccutils_expose_private_constructors.h"
#include "ccutils_views.h"
#include "not_null.hpp"
#include "sqldb_i_query_builder.h"
#include "sqldb_query_builders_common.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
class QueryBuilderFacade;

namespace query_builder_facade {
/**
 * @brief Part of query builder facade.
 */
class InsertQueryBuilder {
 public:
  auto WholeRow() -> InsertQueryBuilder &;
  auto IntoTable(Table table) -> InsertQueryBuilder &;
  auto IntoTable(TableDefinition table_definition) -> InsertQueryBuilder &;
  auto IntoColumns(std::vector<Column> columns) -> InsertQueryBuilder &;
  auto IntoColumns(
      const ccutils::ConstView<ColumnDefinition> &column_definitions)
      -> InsertQueryBuilder &;

  [[nodiscard]] auto Build() const -> std::string;

 private:
  friend class ccutils::ExposePrivateConstructorsTo<QueryBuilderFacade,
                                                    InsertQueryBuilder>;

  explicit InsertQueryBuilder(
      cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder);

  cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder_;

  TableVariant table_{};
  ColumnsVariant columns_{};
};
}  // namespace query_builder_facade
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDERS_SQLDB_INSERT_QUERY_BUILDER_H_
