#ifndef STONKS_SQLDB_QUERY_BUILDERS_SQLDB_DELETE_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDERS_SQLDB_DELETE_QUERY_BUILDER_H_

#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "ccutils_expose_private_constructors.h"
#include "not_null.hpp"
#include "sqldb_i_query_builder.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
class QueryBuilderFacade;

namespace query_builder_facade {
/**
 * @brief Part of query builder facade.
 */
class DeleteQueryBuilder {
 public:
  auto FromTable(Table table) -> DeleteQueryBuilder &;
  auto FromTable(const TableDefinition &table_definition)
      -> DeleteQueryBuilder &;
  auto Where(std::string_view where_clause) -> DeleteQueryBuilder &;

  [[nodiscard]] auto Build() const -> std::string;

 private:
  friend class ccutils::ExposePrivateConstructorsTo<QueryBuilderFacade,
                                                    DeleteQueryBuilder>;

  explicit DeleteQueryBuilder(
      cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder);

  cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder_;

  std::optional<Table> table_{};
  std::optional<std::string> where_clause_{};
};
}  // namespace query_builder_facade
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDERS_SQLDB_DELETE_QUERY_BUILDER_H_
