#ifndef STONKS_SQLDB_SQLDB_I_QUERY_BUILDER_H_
#define STONKS_SQLDB_SQLDB_I_QUERY_BUILDER_H_

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "sqldb_types.h"

namespace stonks::sqldb {
/**
 * @brief Generic builder of queries for DB. Each DB implementation provides its
 * own query builder.
 * @remark Built queries contain placeholders for specified columns and are used
 * by prepared statements.
 * @remark Complex queries can be composed manually.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IQueryBuilder {
 public:
  virtual ~IQueryBuilder() = default;

  /**
   * @brief Builds a query which creates a table from definition if it doesn't
   * exist.
   */
  [[nodiscard]] virtual auto BuildCreateTableIfNotExistsQuery(
      const TableDefinition &table_definition) const -> std::string = 0;

  /**
   * @brief Builds a query which drops the table.
   */
  [[nodiscard]] virtual auto BuildDropTableQuery(const Table &table) const
      -> std::string = 0;

  /**
   * @brief Builds a query which selects specified columns from the table.
   * @param columns Columns to select, or all columns if null.
   * @param where_clause Appended to the query. Should start with "WHERE ".
   */
  [[nodiscard]] virtual auto BuildSelectQuery(
      const Table &table, const std::vector<Column> *columns,
      std::string_view where_clause) const -> std::string = 0;

  /**
   * @brief Builds a query which would insert the values to the specified
   * columns.
   * @remark Not specified columns would get null values inserted.
   */
  [[nodiscard]] virtual auto BuildInsertQuery(
      const Table &table, const std::vector<Column> &columns) const
      -> std::string = 0;

  /**
   * @brief Builds a query which would update the values in the specified
   * columns.
   * @remark Not specified columns would get null values inserted.
   * @param where_clause Appended to the query. Should start with "WHERE ".
   */
  [[nodiscard]] virtual auto BuildUpdateQuery(
      const Table &table, const std::vector<Column> &columns,
      std::string_view where_clause) const -> std::string = 0;

  /**
   * @brief Builds a query which would delete the rows determined by where
   * clause.
   * @param where_clause Appended to the query. Should start with "WHERE ".
   * All rows would be deleted if empty.
   */
  [[nodiscard]] virtual auto BuildDeleteQuery(
      const Table &table, std::string_view where_clause) const
      -> std::string = 0;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_I_QUERY_BUILDER_H_
