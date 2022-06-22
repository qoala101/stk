#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_SELECT_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_SELECT_QUERY_BUILDER_H_

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
class SelectQueryBuilder {
 public:
  explicit SelectQueryBuilder(std::shared_ptr<IQueryBuilder> query_builder);

  auto Columns(std::vector<Column> columns) -> SelectQueryBuilder &;
  auto AllColumns() -> SelectQueryBuilder &;
  auto FromTable(Table table) -> SelectQueryBuilder &;
  auto FromTable(const TableDefinition &table_definition)
      -> SelectQueryBuilder &;
  auto Where(std::string_view where_clause) -> SelectQueryBuilder &;

  [[nodiscard]] auto Build() const -> std::string;

 private:
  std::shared_ptr<IQueryBuilder> query_builder_{};
  std::optional<Table> table_{};
  ColumnsVariant columns_{};
  std::optional<std::string> where_clause_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_SELECT_QUERY_BUILDER_H_
