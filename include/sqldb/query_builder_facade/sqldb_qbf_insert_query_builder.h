#ifndef STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_INSERT_QUERY_BUILDER_H_
#define STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_INSERT_QUERY_BUILDER_H_

#include <fmt/format.h>

#include <vector>

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "cpp_views.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_qbf_columns_variant.h"
#include "sqldb_qbf_table_variant.h"
#include "sqldb_traits.h"
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

  explicit InsertQueryBuilder(cpp::NnUp<IQueryBuilder> query_builder);

  cpp::NnUp<IQueryBuilder> query_builder_;

  TableVariant table_{};
  ColumnsVariant columns_{};
};

class InsertQueryBuilderTemplate {
 public:
  template <typename Column>
  [[nodiscard]] auto Value(std::string TEMP) -> auto & {
    if (!column_names_.empty()) {
      column_names_ += ", ";
    }

    column_names_ += ColumnTraits<Column>::GetName();

    if (!values_.empty()) {
      values_ += ", ";
    }

    values_ += TEMP;

    return *this;
  }

  template <typename Table>
  [[nodiscard]] auto Into() -> auto & {
    Expects(table_name_.empty());
    table_name_ = TableTraits<Table>::GetName();
    Ensures(!table_name_.empty());
    return *this;
  }

  [[nodiscard]] auto Build() const -> Query;

 private:
  std::string column_names_{};
  std::string values_{};
  std::string table_name_{};
};
}  // namespace qbf
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_QUERY_BUILDER_FACADE_SQLDB_QBF_INSERT_QUERY_BUILDER_H_
