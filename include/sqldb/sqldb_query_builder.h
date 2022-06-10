#ifndef STONKS_SQL_DB_SQL_DB_QUERY_BUILDER_H_
#define STONKS_SQL_DB_SQL_DB_QUERY_BUILDER_H_

#include <string>
#include <string_view>
#include <vector>

#include "sqldb_types.h"

namespace stonks::sqldb {
/**
 * @brief Generic builder of queries for DB. Each DB implementation provides its
 * own query builder.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class QueryBuilder {
 public:
  virtual ~QueryBuilder() = default;

  [[nodiscard]] virtual auto BuildCreateTableIfNotExistsQuery(
      const TableDefinition &table_definition) const -> std::string = 0;

  [[nodiscard]] virtual auto BuildDropTableQuery(const Table &table)
      -> std::string = 0;

  [[nodiscard]] auto BuildSelectQuery(
      const TableDefinition &table_definition) const -> std::string;

  [[nodiscard]] auto BuildSelectQuery(const Table &table) const -> std::string;

  [[nodiscard]] virtual auto BuildSelectQuery(
      const Table &table, std::string_view where_clause) const
      -> std::string = 0;

  [[nodiscard]] auto BuildInsertQuery(
      const TableDefinition &table_definition) const -> std::string;

  [[nodiscard]] virtual auto BuildInsertQuery(
      const Table &table, const std::vector<Column> &columns) const
      -> std::string = 0;

  [[nodiscard]] auto BuildUpdateQuery(const TableDefinition &table_definition,
                                      std::string_view where_clause)
      -> std::string;

  [[nodiscard]] virtual auto BuildUpdateQuery(
      const Table &table, const std::vector<Column> &columns,
      std::string_view where_clause) -> std::string = 0;

  [[nodiscard]] virtual auto BuildDeleteQuery(const Table &table,
                                              std::string_view where_clause)
      -> std::string = 0;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQL_DB_SQL_DB_QUERY_BUILDER_H_
