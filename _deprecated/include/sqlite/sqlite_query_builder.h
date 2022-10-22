#ifndef STONKS_SQLITE_SQLITE_QUERY_BUILDER_H_
#define STONKS_SQLITE_SQLITE_QUERY_BUILDER_H_

#include <string_view>
#include <vector>

#include "sqldb_i_query_builder.h"
#include "sqldb_types.h"

namespace stonks::sqlite {
/**
 * @copydoc sqldb::IQueryBuilder
 */
class QueryBuilder : public sqldb::IQueryBuilder {
 public:
  /**
   * @copydoc sqldb::IQueryBuilder::BuildCreateTableIfNotExistsQuery
   */
  [[nodiscard]] auto BuildCreateTableIfNotExistsQuery(
      const sqldb::TableDefinition &table_definition) const
      -> sqldb::Query override;

  /**
   * @copydoc sqldb::IQueryBuilder::BuildDropTableQuery
   */
  [[nodiscard]] auto BuildDropTableQuery(const sqldb::Table &table) const
      -> sqldb::Query override;

  /**
   * @copydoc sqldb::IQueryBuilder::BuildSelectQuery
   */
  [[nodiscard]] auto BuildSelectQuery(const sqldb::Table &table,
                                      const std::vector<sqldb::Column> *columns,
                                      std::string_view where_clause) const
      -> sqldb::Query override;

  /**
   * @copydoc sqldb::IQueryBuilder::BuildInsertQuery
   */
  [[nodiscard]] auto BuildInsertQuery(
      const sqldb::Table &table,
      const std::vector<sqldb::Column> &columns) const -> sqldb::Query override;

  /**
   * @copydoc sqldb::IQueryBuilder::BuildUpdateQuery
   */
  [[nodiscard]] auto BuildUpdateQuery(const sqldb::Table &table,
                                      const std::vector<sqldb::Column> &columns,
                                      std::string_view where_clause) const
      -> sqldb::Query override;

  /**
   * @copydoc sqldb::IQueryBuilder::BuildDeleteQuery
   */
  [[nodiscard]] auto BuildDeleteQuery(const sqldb::Table &table,
                                      std::string_view where_clause) const
      -> sqldb::Query override;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_QUERY_BUILDER_H_
