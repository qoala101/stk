#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_UPDATE_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_UPDATE_QUERY_BUILDER_H_

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "not_null.hpp"
#include "sqldb_i_query_builder.h"
#include "sqldb_query_builders_common.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
/**
 * @brief Part of query builder facade.
 */
class UpdateQueryBuilder {
 public:
  explicit UpdateQueryBuilder(
      cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder);

  auto OfTable(Table table) -> UpdateQueryBuilder &;
  auto OfTable(TableDefinition table_definition) -> UpdateQueryBuilder &;
  auto Columns(std::vector<Column> columns) -> UpdateQueryBuilder &;
  auto Columns(const ConstView<ColumnDefinition> &column_definitions)
      -> UpdateQueryBuilder &;
  auto AllColumns() -> UpdateQueryBuilder &;
  auto Where(std::string_view where_clause) -> UpdateQueryBuilder &;

  [[nodiscard]] auto Build() const -> std::string;

 private:
  cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder_;

  TableVariant table_{};
  ColumnsVariant columns_{};
  std::optional<std::string> where_clause_{};
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_UPDATE_QUERY_BUILDER_H_
