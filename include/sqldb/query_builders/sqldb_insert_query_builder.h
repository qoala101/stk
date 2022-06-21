#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_INSERT_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_INSERT_QUERY_BUILDER_H_

#include <memory>
#include <string>
#include <vector>

#include "sqldb_query_builder.h"
#include "sqldb_query_builders_common.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
/**
 * @brief Part of query builder facade.
 */
class InsertQueryBuilder {
 public:
  explicit InsertQueryBuilder(std::shared_ptr<IQueryBuilder> query_builder);

  auto IntoTable(Table table) -> InsertQueryBuilder &;
  auto IntoTable(TableDefinition table_definition) -> InsertQueryBuilder &;
  auto IntoColumns(std::vector<Column> columns) -> InsertQueryBuilder &;
  
  [[nodiscard]] auto Build() const -> std::string;

 private:
  std::shared_ptr<IQueryBuilder> query_builder_{};
  TableVariant table_{};
  ColumnsVariant columns_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_INSERT_QUERY_BUILDER_H_
