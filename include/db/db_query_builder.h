#ifndef STONKS_DB_DB_QUERY_BUILDER_H_
#define STONKS_DB_DB_QUERY_BUILDER_H_

#include "db_types.h"

namespace stonks::db {
/**
 * @brief Generic builder of queries for DB. Each DB implementation provides its
 * own query builder.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class QueryBuilder {
 public:
  virtual ~QueryBuilder() = default;

  [[nodiscard]] virtual auto BuildInsertQuery(
      const TableDefinition &table_definition) const -> std::string = 0;

  [[nodiscard]] virtual auto BuildInsertQuery(
      const Table &table, const std::vector<Column> &columns) const
      -> std::string = 0;

  [[nodiscard]] virtual auto BuildSelectQuery(const Table &table) const
      -> std::string = 0;
};
}  // namespace stonks::db

#endif  // STONKS_DB_DB_QUERY_BUILDER_H_
