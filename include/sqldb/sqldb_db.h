#ifndef STONKS_SQLDB_SQLDB_DB_H_
#define STONKS_SQLDB_SQLDB_DB_H_

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "sqldb_query_builder.h"
#include "sqldb_row_definition.h"
#include "sqldb_select_statement.h"
#include "sqldb_types.h"
#include "sqldb_update_statement.h"

namespace stonks::sqldb {
/**
 * @brief Generic DB interface.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class Db {
 public:
  virtual ~Db() = default;

  /**
   * @brief Creates prepared statement which can then be called to execute the
   * select query on DB.
   */
  [[nodiscard]] virtual auto PrepareStatement(
      std::string_view query, const RowDefinition &result_definition)
      -> std::unique_ptr<ISelectStatement> = 0;

  /**
   * @brief Creates prepared statement which can then be called to execute the
   * update query on DB.
   */
  [[nodiscard]] virtual auto PrepareStatement(std::string_view query)
      -> std::unique_ptr<IUpdateStatement> = 0;

  /**
   * @brief Stores this DB to the specified file.
   */
  virtual void WriteToFile(std::string_view file_path) = 0;
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_DB_H_
