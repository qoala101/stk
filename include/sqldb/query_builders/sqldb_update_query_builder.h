#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_UPDATE_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_UPDATE_QUERY_BUILDER_H_

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "sqldb_query_builder.h"
#include "sqldb_query_builders_common.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
/**
 * @brief Part of query builder facade.
 */
class UpdateQueryBuilder {
 public:
  explicit UpdateQueryBuilder(std::shared_ptr<IQueryBuilder> query_builder);

  auto Table(Table table) -> UpdateQueryBuilder &;
  auto Table(TableDefinition table_definition) -> UpdateQueryBuilder &;
  auto Columns(std::vector<Column> columns) -> UpdateQueryBuilder &;
  auto AllColumns() -> UpdateQueryBuilder &;
  auto Where(std::string_view where_clause) -> UpdateQueryBuilder &;
  
  [[nodiscard]] auto Build() const -> std::string;

 private:
  std::shared_ptr<IQueryBuilder> query_builder_{};
  TableVariant table_{};
  ColumnsVariant columns_{};
  std::optional<std::string> where_clause_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_UPDATE_QUERY_BUILDER_H_
