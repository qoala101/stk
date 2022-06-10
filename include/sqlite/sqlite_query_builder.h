#ifndef STONKS_DB_SQLITE_SQLITE_QUERY_BUILDER_H_
#define STONKS_DB_SQLITE_SQLITE_QUERY_BUILDER_H_

#include <string>
#include <string_view>
#include <vector>

#include "db_query_builder.h"
#include "db_types.h"

namespace stonks::db::sqlite {
/**
 * @copydoc QueryBuilder
 */
class SqliteQueryBuilder : public QueryBuilder {
 public:
  /**
   * @copydoc QueryBuilder::BuildCreateTableIfNotExistsQuery
   */
  [[nodiscard]] auto BuildCreateTableIfNotExistsQuery(
      const TableDefinition &table_definition) const -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildCreateTableIfNotExistsQuery
   */
  [[nodiscard]] auto BuildDropTableQuery(const Table &table)
      -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildSelectQuery
   */
  [[nodiscard]] auto BuildSelectQuery(const Table &table,
                                      std::string_view where_clause) const
      -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildInsertQuery
   */
  [[nodiscard]] auto BuildInsertQuery(const Table &table,
                                      const std::vector<Column> &columns) const
      -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildCreateTableIfNotExistsQuery
   */
  [[nodiscard]] auto BuildUpdateQuery(const Table &table,
                                      const std::vector<Column> &columns,
                                      std::string_view where_clause)
      -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildCreateTableIfNotExistsQuery
   */
  [[nodiscard]] auto BuildDeleteQuery(const Table &table,
                                      std::string_view where_clause)
      -> std::string override;
};
}  // namespace stonks::db::sqlite

#endif  // STONKS_DB_SQLITE_SQLITE_QUERY_BUILDER_H_
