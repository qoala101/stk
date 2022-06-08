#ifndef STONKS_DB_SQLITE_SQLITE_QUERY_BUILDER_H_
#define STONKS_DB_SQLITE_SQLITE_QUERY_BUILDER_H_

#include <string>
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
   * @copydoc QueryBuilder::BuildInsertQuery
   */
  [[nodiscard]] auto BuildInsertQuery(
      const TableDefinition &table_definition) const -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildInsertQuery
   */
  [[nodiscard]] auto BuildInsertQuery(const Table &table,
                                      const std::vector<Column> &columns) const
      -> std::string override;

  /**
   * @copydoc QueryBuilder::BuildSelectQuery
   */
  [[nodiscard]] auto BuildSelectQuery(const Table &table) const
      -> std::string override;
};
}  // namespace stonks::db::sqlite

#endif  // STONKS_DB_SQLITE_SQLITE_QUERY_BUILDER_H_
