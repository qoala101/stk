#ifndef STONKS_DB_DB_H_
#define STONKS_DB_DB_H_

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "db_prepared_statement.h"
#include "db_query_builder.h"
#include "db_types.h"

namespace stonks::db {
/**
 * @brief Generic DB interface.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class Db {
 public:
  virtual ~Db() = default;

  /**
   * @brief Creates a table if no tables with the same name exist.
   */
  virtual void CreateTableIfNotExists(
      const TableDefinition &table_definition) = 0;

  /**
   * @brief Deletes a table.
   */
  virtual void DropTable(const Table &table) = 0;

  /**
   * @brief Deletes all the rows in the table found by the WHERE clause.
   * @param where_clause Should start from "WHERE " or be empty.
   */
  virtual void DeleteFrom(const Table &table,
                          std::string_view where_clause) = 0;

  /**
   * @brief Updates single row determined by where.
   * @param where_clause Should start from "WHERE" and point at a single row.
   */
  virtual void UpdateRow(const Table &table, const Row &new_row_values,
                         std::string_view where_clause) = 0;

  /**
   * @brief Creates prepared statement which can then be called to execute the
   * query on DB.
   */
  [[nodiscard]] virtual auto PrepareStatement(std::string_view query)
      -> std::unique_ptr<PreparedStatement> = 0;

  /**
   * @brief Creates query builder which knows how to build queries for this DB.
   */
  [[nodiscard]] virtual auto CreateQueryBuilder()
      -> std::unique_ptr<QueryBuilder> = 0;
};
}  // namespace stonks::db

#endif  // STONKS_DB_DB_H_
