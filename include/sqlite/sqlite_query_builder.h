#ifndef STONKS_SQLITE_SQLITE_QUERY_BUILDER_H_
#define STONKS_SQLITE_SQLITE_QUERY_BUILDER_H_

#include <string>
#include <string_view>
#include <vector>

#include "sqldb_query_builder.h"
#include "sqldb_types.h"

namespace stonks::sqlite {
/**
 * @copydoc QueryBuilder
 */
class SqliteQueryBuilder : public sqldb::QueryBuilder {
 public:
  /**
   * @copydoc QueryBuilder::BuildCreateTableIfNotExistsQuery
   */
  [[nodiscard]] auto BuildCreateTableIfNotExistsQuery(
      const sqldb::TableDefinition &table_definition) const
      -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildCreateTableIfNotExistsQuery
   */
  [[nodiscard]] auto BuildDropTableQuery(const sqldb::Table &table)
      -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildSelectQuery
   */
  [[nodiscard]] auto BuildSelectQuery(const sqldb::Table &table,
                                      const std::vector<sqldb::Column> *columns,
                                      std::string_view where_clause) const
      -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildInsertQuery
   */
  [[nodiscard]] auto BuildInsertQuery(
      const sqldb::Table &table,
      const std::vector<sqldb::Column> &columns) const -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildCreateTableIfNotExistsQuery
   */
  [[nodiscard]] auto BuildUpdateQuery(const sqldb::Table &table,
                                      const std::vector<sqldb::Column> &columns,
                                      std::string_view where_clause)
      -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildCreateTableIfNotExistsQuery
   */
  [[nodiscard]] auto BuildDeleteQuery(const sqldb::Table &table,
                                      std::string_view where_clause)
      -> std::string override;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_QUERY_BUILDER_H_
