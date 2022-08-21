#ifndef STONKS_SQLDB_QUERY_BUILDERS_SQLDB_UPDATE_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDERS_SQLDB_UPDATE_QUERY_BUILDER_H_

#include <optional>
#include <string>
#include <string_view>
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
class UpdateQueryBuilder {
 public:
  auto OfTable(Table table) -> UpdateQueryBuilder &;
  auto OfTable(TableDefinition table_definition) -> UpdateQueryBuilder &;

  auto Columns(std::vector<Column> columns) -> UpdateQueryBuilder &;
  auto Columns(const ccutils::ConstView<ColumnDefinition> &column_definitions)
      -> UpdateQueryBuilder &;
  auto AllColumns() -> UpdateQueryBuilder &;

  auto Where(std::string_view where_clause) -> UpdateQueryBuilder &;

  [[nodiscard]] auto Build() const -> std::string;

 private:
  friend class ccutils::ExposePrivateConstructorsTo<QueryBuilderFacade,
                                                    UpdateQueryBuilder>;

  explicit UpdateQueryBuilder(ccutils::NnSp<IQueryBuilder> query_builder);

  ccutils::NnSp<IQueryBuilder> query_builder_;

  TableVariant table_{};
  ColumnsVariant columns_{};
  std::optional<std::string> where_clause_{};
};
}  // namespace query_builder_facade
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDERS_SQLDB_UPDATE_QUERY_BUILDER_H_
