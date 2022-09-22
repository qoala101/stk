#ifndef STONKS_SQLDB_SQLDB_QUERY_BUILDER_FACADE_H_
#define STONKS_SQLDB_SQLDB_QUERY_BUILDER_FACADE_H_

#include "cpp_not_null.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_qbf_delete_query_builder.h"
#include "sqldb_qbf_insert_query_builder.h"
#include "sqldb_qbf_select_query_builder.h"
#include "sqldb_qbf_update_query_builder.h"

namespace stonks::sqldb {
/**
 * @brief Start point for query builders which provide fluent query-building
 * API.
 */
class QueryBuilderFacade {
 public:
  explicit QueryBuilderFacade(cpp::NnSp<IQueryBuilder> query_builder);

  /**
   * @brief Start building select statement.
   */
  [[nodiscard]] auto Select() const -> qbf::SelectQueryBuilder;

  /**
   * @brief Start building insert statement.
   */
  [[nodiscard]] auto Insert() const -> qbf::InsertQueryBuilder;

  /**
   * @brief Start building update statement.
   */
  [[nodiscard]] auto Update() const -> qbf::UpdateQueryBuilder;

  /**
   * @brief Start building delete statement.
   */
  [[nodiscard]] auto Delete() const -> qbf::DeleteQueryBuilder;

 private:
  cpp::NnSp<IQueryBuilder> query_builder_;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_QUERY_BUILDER_FACADE_H_
