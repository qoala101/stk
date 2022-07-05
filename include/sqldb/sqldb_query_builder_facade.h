#ifndef STONKS_SQLDB_SQLDB_QUERY_BUILDER_FACADE_H_
#define STONKS_SQLDB_SQLDB_QUERY_BUILDER_FACADE_H_

#include <memory>

#include "not_null.hpp"
#include "sqldb_delete_query_builder.h"
#include "sqldb_insert_query_builder.h"
#include "sqldb_query_builder.h"
#include "sqldb_select_query_builder.h"
#include "sqldb_update_query_builder.h"

namespace stonks::sqldb {
/**
 * @brief Start point for query builders which provide fluent query-building
 * API.
 */
class QueryBuilderFacade {
 public:
  explicit QueryBuilderFacade(
      cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder);

  [[nodiscard]] auto Select() const -> SelectQueryBuilder;
  [[nodiscard]] auto Insert() const -> InsertQueryBuilder;
  [[nodiscard]] auto Update() const -> UpdateQueryBuilder;
  [[nodiscard]] auto Delete() const -> DeleteQueryBuilder;

 private:
  cpp::not_null<std::shared_ptr<IQueryBuilder>> query_builder_;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_QUERY_BUILDER_FACADE_H_
