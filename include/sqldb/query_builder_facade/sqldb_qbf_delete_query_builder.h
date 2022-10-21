#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_DELETE_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_DELETE_QUERY_BUILDER_H_

#include <gsl/assert>
#include <string>
#include <string_view>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb {
class QueryBuilderFacade;

namespace qbf {
/**
 * @brief Part of query builder facade.
 */
class DeleteQueryBuilder {
 public:
  auto FromTable(Table table) -> DeleteQueryBuilder &;
  auto FromTable(const TableDefinition &table_definition)
      -> DeleteQueryBuilder &;
  auto Where(std::string_view where_clause) -> DeleteQueryBuilder &;

  [[nodiscard]] auto Build() const -> Query;

 private:
  friend class cpp::ExposePrivateConstructorsTo<QueryBuilderFacade,
                                                DeleteQueryBuilder>;

  explicit DeleteQueryBuilder(cpp::NnUp<IQueryBuilder> query_builder);

  cpp::NnUp<IQueryBuilder> query_builder_;

  cpp::Opt<Table> table_{};
  cpp::Opt<std::string> where_clause_{};
};

class DeleteQueryBuilderTemplate {
 public:
  template <typename Table>
  [[nodiscard]] auto From() -> auto & {
    Expects(table_name_.empty());
    table_name_ = TableTraits<Table>::GetName();
    Ensures(!table_name_.empty());
    return *this;
  }

  [[nodiscard]] auto Where(std::string_view where_clause)
      -> DeleteQueryBuilderTemplate &;
  [[nodiscard]] auto And(std::string_view where_clause)
      -> DeleteQueryBuilderTemplate &;
  [[nodiscard]] auto Or(std::string_view where_clause)
      -> DeleteQueryBuilderTemplate &;

  [[nodiscard]] auto Build() const -> Query;

 private:
  std::string table_name_{};
  std::string where_clause_{};
};
}  // namespace qbf
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_DELETE_QUERY_BUILDER_H_
